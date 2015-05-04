

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/capability.h>
#include <linux/fcntl.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/netdevice.h>
#include <linux/if_packet.h>
#include <linux/wireless.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <net/net_namespace.h>
#include <net/ip.h>
#include <net/protocol.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <linux/errno.h>
#include <linux/timer.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/ioctls.h>
#include <asm/page.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/poll.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mutex.h>

#include <linux/crypto.h>


#define  ETH_TYPE_8021X_AUTH  0x888e    //8021x认证的协议类型号。
#ifndef ETH_P_PAE   
#define  ETH_P_PAE 0x888e
#endif

//
struct auth_1x_hdr{
    u8 ver;
    u8 type;
    u16 len;
};
enum ath_1x_hdr_type
{
    EAP_PKT = 0,
    EAPOL_START = 1,
    EAPOL_LOGOFF,
    EAPOL_KEY,
};
enum EAP_HDR_CODE 
{
	EAP_HDR_CODE_REQUEST  = 1,
	EAP_HDR_CODE_RESPONSE = 2,
	EAP_HDR_CODE_SUCCESS = 3,
	EAP_HDR_CODE_FAIL = 4,
};
struct eap_hdr{
	u8 code;	/* 1 request 2 response */
	u8 id;  	/* seq id num */
	u16 len;
};
struct eap_body{
	u8 type;
	u8 body[0];
};

struct eap_md5{
	u8 len;
	char value[16];
	char extra[0];//usrname.
};
typedef enum 
{
	EAP_BODY_TYPE_IDENTITY =1,
	EAP_BODY_TYPE_NOTIFICATION=2,
	EAP_BODY_TYPE_MD5=4,
	EAP_BODY_TYPE_AVAILABLE=20
}eap_bodytype;

struct fpp_1xclient_t
{
/* config info from usr layer */
  char usrname[64];
  char password[64];
  char port[32];
  int  flag; /* 1;enable,0,disable */
  
/* handler resuest */
 // struct sk_buff* pkt;   /* resuest pkt pointer */
  struct work_struct  respon_work;
  struct timer_list   start_timer;   /* eapol start's timer */
  struct timer_list   state_timer;   /* auth stat timer */
  int    state;/* 0 fail, 1,success */
  
	struct auth_1x_hdr  ahdr;
	struct eap_hdr eaph;
	struct eap_body eapb;
	struct eap_md5 eap_md5;
	struct crypto_hash *hash_tfm;
	
/*  */
};
#if 0
/* hook函数 */
Struct nf_hook_ops fpp_1xclient_hooks[] = {
    .hook = fpp_1xclient_hook,
    .hooknum = NF_BR_IN,
    .priority = NF_BF_PRI_BRNF,
};

/* 配置的处理函数 */
Static struct fpp_msg_type _fpp_1xclient_netlink_type = {
.type = FPP_NL_TYPE_1XCLIENT,
.func = _fpp_1xclient_netlink_process
#endif


/*****************全局变量*******************************/
struct fpp_1xclient_t  g_fpp_1xclient;
u8        auth_multcast_mac[6] = {0x01,0x80,0xc2,0x00,0x00,0x03};  //认证使用的组播地址。

/*********************************function********************************/

static int _calculate_md5(char *dst,
					struct fpp_1xclient_t* fpp_1xclient,
				  char *src, int len);

/*********************************function********************************/

static int _calculate_md5(char *dst,
					struct fpp_1xclient_t* fpp_1xclient,
				  char *src, int len)
{
	struct scatterlist sg;
	struct hash_desc desc = {fpp_1xclient->hash_tfm,0};
	int rc = 0;
	sg_init_one(&sg, (u8 *)src, len);
	if (!desc.tfm) 
	{
		desc.tfm = crypto_alloc_hash("md5", 0, CRYPTO_ALG_ASYNC);
		if (IS_ERR(desc.tfm)) {
			rc = PTR_ERR(desc.tfm);
			printk(KERN_ERR "Error attempting to "
					"allocate crypto context; rc = [%d]\n",
					rc);
			goto out;
		}
	}
	rc = crypto_hash_init(&desc);
	if (rc) {
		printk(KERN_ERR
		       "%s: Error initializing crypto hash; rc = [%d]\n",
		       __func__, rc);
		goto out;
	}
	rc = crypto_hash_update(&desc, &sg, len);
	if (rc) {
		printk(KERN_ERR
		       "%s: Error updating crypto hash; rc = [%d]\n",
		       __func__, rc);
		goto out;
	}
	rc = crypto_hash_final(&desc, dst);
	if (rc) {
		printk(KERN_ERR
		       "%s: Error finalizing crypto hash; rc = [%d]\n",
		       __func__, rc);
		goto out;
	}
	
out:
	return rc;
}

static void fpp_1xclient_timer_state(unsigned long arg)
{
	struct fpp_1xclient_t *data = (struct fpp_1xclient_t *) arg;
	if(data->flag == 1)
	{
		if(data->state != 1)
		{
    		mod_timer(&data->start_timer, round_jiffies(jiffies + HZ));
		}
	}
	
    mod_timer(&data->state_timer, round_jiffies(jiffies + 10*HZ));
	return;
}	

static void fpp_1xclient_timer_start(unsigned long arg)
{
	struct fpp_1xclient_t *_1xclient = (struct fpp_1xclient_t *) arg;
	
	struct sk_buff *skb;
	int len;
	struct net_device* dev;
	struct ethhdr* eh;
	struct auth_1x_hdr*  ahdr;

	dev = dev_get_by_name(&init_net, _1xclient->port);
	if (dev == NULL) {
		return;
	}
	len = sizeof(struct ethhdr)+sizeof(struct auth_1x_hdr)+1;
	skb = alloc_skb (len + LL_RESERVED_SPACE(dev) , GFP_ATOMIC);
	if (skb == NULL)
		return ;

	skb_reserve(skb, LL_RESERVED_SPACE(dev));
	skb_reset_network_header(skb);
	
	eh = (struct ethhdr*) skb_put(skb, sizeof(struct ethhdr));
	eh->h_proto = htons(ETH_P_PAE);
	memcpy(eh->h_dest,auth_multcast_mac,sizeof(auth_multcast_mac));
	memcpy(eh->h_source ,dev->dev_addr,6);
	

	ahdr = (struct auth_1x_hdr*) skb_put(skb,sizeof( struct auth_1x_hdr));
	ahdr->ver = _1xclient->ahdr.ver;
	ahdr->type = EAPOL_START;
	ahdr->len = 0;
	
	
	
	skb->dev = dev;
	skb->protocol = htons(ETH_P_PAE);


	dev_queue_xmit(skb);

	

	return;
}	
int _send_identity_response(struct fpp_1xclient_t* _1xclient)
{
	struct sk_buff *skb;
	int len;
	struct net_device* dev;
	struct ethhdr* eh;
	struct auth_1x_hdr*  ahdr;
	struct eap_hdr* eaph;
	struct eap_body* eapb;

	dev = dev_get_by_name(&init_net, _1xclient->port);
	if (dev == NULL) {
		return -2;
	}
	len = sizeof(struct ethhdr)+sizeof(struct auth_1x_hdr)+sizeof(struct eap_hdr)
			+sizeof(struct eap_body)+strlen(_1xclient->usrname);
	skb = alloc_skb (len + LL_RESERVED_SPACE(dev) , GFP_ATOMIC);
	if (skb == NULL)
		return -1;

	skb_reserve(skb, LL_RESERVED_SPACE(dev));
	skb_reset_network_header(skb);
	
	eh = (struct ethhdr*) skb_put(skb, sizeof(struct ethhdr));
	eh->h_proto = htons(ETH_P_PAE);
	memcpy(eh->h_dest,auth_multcast_mac,sizeof(auth_multcast_mac));
	memcpy(eh->h_source ,dev->dev_addr,6);
	

	ahdr = (struct auth_1x_hdr*) skb_put(skb,sizeof( struct auth_1x_hdr));
	ahdr->ver = _1xclient->ahdr.ver;
	ahdr->type = EAP_PKT;
	ahdr->len = htons(sizeof(struct eap_hdr)+sizeof(struct eap_body)+strlen(_1xclient->usrname));
	
	
	eaph = (struct eap_hdr*) skb_put(skb, sizeof(struct eap_hdr));
	eaph->code = EAP_HDR_CODE_RESPONSE;
	eaph->id = _1xclient->eaph.id;
	eaph->len = ahdr->len;

	eapb = (struct eap_body*) skb_put(skb, sizeof(struct eap_body)+strlen(_1xclient->usrname));
	eapb->type = _1xclient->eapb.type;
	memcpy(eapb->body,_1xclient->usrname,strlen(_1xclient->usrname));
	
	skb->dev = dev;
	skb->protocol = htons(ETH_P_PAE);


	dev_queue_xmit(skb);
	return 0;
	
}
int _send_md5_response(struct fpp_1xclient_t* _1xclient)
{
	struct sk_buff *skb;
	int len;
	struct net_device* dev;
	struct ethhdr* eh;
	struct auth_1x_hdr*  ahdr;
	struct eap_hdr* eaph;
	struct eap_body* eapb;
	struct eap_md5* md5_info;
	u8 src[128];
	int tohashlen;

	dev = dev_get_by_name(&init_net, _1xclient->port);
	if (dev == NULL) {
		return -2;
	}
	len = sizeof(struct ethhdr)
			+sizeof(struct auth_1x_hdr)
			+sizeof(struct eap_hdr)
			+sizeof(struct eap_body)
			+sizeof(struct eap_md5)
			+strlen(_1xclient->usrname);
	skb = alloc_skb (len + LL_RESERVED_SPACE(dev) , GFP_ATOMIC);
	if (skb == NULL)
		return -1;

	skb_reserve(skb, LL_RESERVED_SPACE(dev));
	skb_reset_network_header(skb);
	
	eh = (struct ethhdr*) skb_put(skb, sizeof(struct ethhdr));
	eh->h_proto = htons(ETH_P_PAE);
	memcpy(eh->h_dest,auth_multcast_mac,sizeof(auth_multcast_mac));
	memcpy(eh->h_source ,dev->dev_addr,6);
	

	ahdr = (struct auth_1x_hdr*) skb_put(skb, sizeof(struct auth_1x_hdr));
	ahdr->ver = _1xclient->ahdr.ver;
	ahdr->type = EAP_PKT;
	ahdr->len = htons(sizeof(struct eap_hdr)+sizeof(struct eap_body)+sizeof(struct eap_md5)+strlen(_1xclient->usrname));
	
	
	eaph = (struct eap_hdr*) skb_put(skb, sizeof(struct eap_hdr));
	eaph->code = EAP_HDR_CODE_RESPONSE;
	eaph->id = _1xclient->eaph.id;
	eaph->len = ahdr->len;

	eapb = (struct eap_body*) skb_put(skb, sizeof(struct eap_body));
	eapb->type = _1xclient->eapb.type;
	/*  */

	md5_info = (struct eap_md5*) skb_put(skb, sizeof(struct eap_md5)+strlen(_1xclient->usrname));
	/* id + password + value */
	src[0] = eaph->id;
	memcpy(src+1,_1xclient->password,strlen(_1xclient->password));
	memcpy(src+1+strlen(_1xclient->password),_1xclient->eap_md5.value,16);
	
	tohashlen = (1 + 16 + strlen(_1xclient->password));
	_calculate_md5(md5_info->value,
					_1xclient,
				  src,tohashlen);
	printk("------md5_info->value <%s>-------\n",md5_info->value);
	memcpy(md5_info->extra,_1xclient->usrname,strlen(_1xclient->usrname));
    md5_info->len = 16;
	skb->dev = dev;
	skb->protocol = htons(ETH_P_PAE);


	dev_queue_xmit(skb);
	return 0;
}


void fpp_1xclient_parse_pkt(struct fpp_1xclient_t* data,struct sk_buff* skb)
{
    
	struct auth_1x_hdr*  ahdr;
	struct eap_hdr* eaph;
	struct eap_body* eapb;
	struct eap_md5* eap_md;


    ahdr = (struct auth_1x_hdr*)(skb_mac_header(skb)+sizeof(struct ethhdr));

	
    if (!pskb_may_pull(skb, sizeof(struct auth_1x_hdr))){
		
		goto _error;
    	}
	
    if (!pskb_may_pull(skb,htons(ahdr->len ))){
		
		goto _error;
    	}
	eaph = (struct eap_hdr*)((u8*)ahdr+sizeof(struct auth_1x_hdr));

printk("%s:%d eaph code %d id %d \n",__func__,__LINE__,eaph->code,eaph->id);
	eapb = (struct eap_body*)((u8*)eaph+sizeof(struct eap_hdr));
	memcpy(&data->ahdr,ahdr,sizeof(struct auth_1x_hdr));
	memcpy(&data->eaph,eaph,sizeof(struct eap_hdr));
	
	memcpy(&data->eapb,eapb,sizeof(struct eap_body));
	if(data->eapb.type == EAP_BODY_TYPE_MD5)
	{
		
		eap_md = (struct eap_md5*)((u8*)eapb+sizeof(struct eap_body));
		memcpy(&data->eap_md5,eap_md,sizeof(struct eap_md5));
	}

    printk("%s:%d ahdr.type %d  \n",__func__,__LINE__,g_fpp_1xclient.ahdr.type);

_error:
	return;
}	



void fpp_1xclient_response_work(struct work_struct *work)
{
    struct fpp_1xclient_t* data = container_of(work, struct fpp_1xclient_t, respon_work);



	
	printk("%s:%d eap body type %d  \n",__func__,__LINE__,data->eapb.type);	
	if (data->eapb.type == EAP_BODY_TYPE_IDENTITY)
	{
		_send_identity_response(data);
	}
	else if (data->eapb.type == EAP_BODY_TYPE_MD5)
	{
		_send_md5_response(data);
	}
	else
	{
		printk("%s:%d eap body type %d , \n",__func__,__LINE__,data->eapb.type);
		return;
	}
//_error:
	return;
}

static int fpp_1x_device_event(struct notifier_block *this, unsigned long event,
	void *ptr)
{
    return 0;
}
static int fpp_eap_pkt(struct fpp_1xclient_t* _1xclient)
{
	printk("%s:%d g_fpp_1xclient.eaph.code %d  \n",__func__,__LINE__,_1xclient->eaph.code);
		switch(_1xclient->eaph.code)
		{
			case EAP_HDR_CODE_RESPONSE:  //do nothing
				return NET_RX_SUCCESS;
			case EAP_HDR_CODE_SUCCESS:
				_1xclient->state = 1;
				return NET_RX_SUCCESS;
			case EAP_HDR_CODE_FAIL:
				_1xclient->state = 0;
				return NET_RX_SUCCESS;
				
			case EAP_HDR_CODE_REQUEST:
	            schedule_work(&_1xclient->respon_work);
		}
	return 0;
}
int eapol_skb_recv(struct sk_buff *skb, struct net_device *dev,
		  struct packet_type *ptype, struct net_device *orig_dev)
{
//    const unsigned char *dest = eth_hdr(skb)->h_dest;
//    const unsigned char *src = eth_hdr(skb)->h_source;
    struct auth_1x_hdr*  ahdr;
    if (!pskb_may_pull(skb, sizeof(struct auth_1x_hdr)))
		goto _error;

    ahdr = (struct auth_1x_hdr*)(skb_mac_header(skb)+sizeof(struct ethhdr));

	fpp_1xclient_parse_pkt(&g_fpp_1xclient,skb);
	
    printk("%s:%d ahdr.type %d  \n",__func__,__LINE__,g_fpp_1xclient.ahdr.type);
	switch(g_fpp_1xclient.ahdr.type)
    {
        case EAP_PKT:
			fpp_eap_pkt(&g_fpp_1xclient);
			printk("%s:%d schedule work ----------\n",__func__,__LINE__);
            break;
            
        case EAPOL_START:
            
            printk("%s:%d recv START ,error\n",__func__,__LINE__);
            break;
        case EAPOL_LOGOFF:
            break;
        case EAPOL_KEY:
            break;
        default:
            printk("%s:%d unknown type ,error\n",__func__,__LINE__);
     }
	
    
  

 _error : 
    return NET_RX_SUCCESS;
}		  

/**************************net arch*********************/
static struct packet_type eapol_pt   = {
	.type = cpu_to_be16(ETH_P_PAE),
	.func = eapol_skb_recv, /*  receive method */
};
/* 只关注eth0的up和br-wan的NETDEV_CHANGEADDR */
static struct notifier_block fpp_1xclient_notifier_block  = {
  .notifier_call = fpp_1x_device_event,
};

/****************init and cleanup ************/
static int __init eapol_proto_init(void)
{
    dev_add_pack (&eapol_pt);
    register_netdevice_notifier (&fpp_1xclient_notifier_block);
    
    setup_timer(&g_fpp_1xclient.start_timer, fpp_1xclient_timer_start,(unsigned long) &g_fpp_1xclient);
    setup_timer(&g_fpp_1xclient.state_timer, fpp_1xclient_timer_state,(unsigned long) &g_fpp_1xclient);
	
    INIT_WORK(&g_fpp_1xclient.respon_work, fpp_1xclient_response_work);		      
    ////////////
    strcpy(g_fpp_1xclient.usrname,"huawei");
    strcpy(g_fpp_1xclient.password,"huawei");
    strcpy(g_fpp_1xclient.port,"eth1");
    g_fpp_1xclient.flag = 1;//enable
    g_fpp_1xclient.state = 0;//fail.
    mod_timer(&g_fpp_1xclient.state_timer, round_jiffies(jiffies + HZ/4));
    return 0;
}

static void __exit eapol_cleanup_module(void)
{
    dev_remove_pack(&eapol_pt);
    unregister_netdevice_notifier(&fpp_1xclient_notifier_block);
	del_timer(&g_fpp_1xclient.start_timer);
	del_timer(&g_fpp_1xclient.state_timer);
	
	if(g_fpp_1xclient.hash_tfm)
		crypto_free_hash(g_fpp_1xclient.hash_tfm);
	
}


module_init(eapol_proto_init);
module_exit(eapol_cleanup_module);

MODULE_LICENSE("GPL");

