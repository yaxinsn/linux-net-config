

/******************************************
*
* 本文件的程序主要是用来抓 SIP等报文。
* 本程序抓sip的报文，并解析出sip里的call-id,以此作为key,把pkt的过程放到
一个session中，session放到一个全局的链表中。
跟踪每个session的从生到死的过程。最后free session.

* 
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "log.h"
#include <arpa/inet.h>

#include <pthread.h>    

#include "sniffer_lib.h"  

#include <linux/if_ether.h> //struct ethhdr
//#include <linux/ip.h> //struct iphdr
#include <linux/udp.h>//struct udphdr
//#include <linux/in.h>
#include "config.h"
#include "linux-utils.h"
#include "wake_utils.h"
#include "sniffer_sip.h"
#include "str_lib.h"

#include "sniffer_rtp.h"

extern struct config_st g_config;


struct sip_ctx_t
{
    pthread_mutex_t head_lock;  //sync

    struct list_head si_head;

};
struct sip_ctx_t sip_ctx;


struct session_info* _si_new_session()
{
    struct session_info* ss =NULL;
    ss = (struct session_info*)malloc(sizeof(struct session_info));
    if ( ss == NULL)
        return NULL;
        
    pthread_mutex_lock(&sip_ctx.head_lock);
    list_add(&ss->node,&sip_ctx.si_head);
    pthread_mutex_unlock(&sip_ctx.head_lock);
    return ss;
}
void _si_del_session(struct session_info* si)
{
    FREE(si->call_id);
    pthread_mutex_lock(&sip_ctx.head_lock);

    list_del(&si->node);
    
        pthread_mutex_unlock(&sip_ctx.head_lock);
    FREE(si);
    return;
}
struct session_info* _si_find_session(char* call_id)
{
    struct session_info* p;
    struct session_info* n;
    struct list_head* si_head;
    si_head = &sip_ctx.si_head;
    log("debug callid %s \n",call_id);
    
    list_for_each_entry_safe(p,n,si_head,node)
    {

        if(!strcmp(call_id,p->call_id))
        {
            return p;
        }
    }
    return NULL;
}

/*
sniffer_handle_sip 一个包一个包地进行处理。
每抓一个包被调用一次。

struct pcap_pkthdr  
{  
    struct timeval ts; // 抓到包的时间  
    bpf_u_int32 caplen; // 表示抓到的数据长度  
    bpf_u_int32 len; // 表示数据包的实际长度  
}  


*/
/****************************************** check and parse **************************************/
int check_iphdr( const struct pcap_pkthdr * phdr, const u_char * pkt,
    struct iphdr** iphdr_p )
{

	const struct ethhdr* ethh = NULL;
	const struct iphdr* iph;
	const u8*   data = pkt;
	const u16* vlan_proto = 0;
	if (phdr->len != phdr->caplen)
	{
		log(" phdr->len %d != phdr->caplen %d \n",phdr->len,phdr->caplen);
		return -1;
	}
	ethh =(struct ethhdr*) data;
	if(ethh->h_proto != htons(ETH_P_IP))
	{
		if(ethh->h_proto != htons(ETH_P_8021Q))
		{
			vlan_proto = &ethh->h_proto;
			vlan_proto += 2;// shift 4 byte;
			if(*vlan_proto != htons(ETH_P_IP)){
				log("pkt is  vlan pkt,but not a vlanip pkt; vlan_proto %x \n",htons(*vlan_proto));
				return -1;
			}
			else
				iph = (struct iphdr*)vlan_proto+1;
		}
		else
		{
			log("pkt is not ip or vlan pkt; ethh->h_proto %x \n",htons(ethh->h_proto));
			return -1;
		}
	}
	else
	{
		iph = (struct iphdr*)(ethh+1);
	}
	/* ip hdr is corrent */
	if(iph->ihl <5 || iph->version != 4){
	
		log("ip header error\n");
		goto inhdr_error;
	}
	if(iph->ihl*4 >  phdr->caplen -((u8*)iph - data)){
		log("ip header len is beyond the caplen!\n");
		goto inhdr_error;
	}
	if(ntohs(iph->tot_len) > phdr->caplen -((u8*)iph - data))
	{
		log(" ip tot len is beyond the caplen! \n");
		goto inhdr_error;
	}
	*iphdr_p = iph;
	
	return 0;
inhdr_error:
	
	return -2;
}
int check_udp( struct iphdr* iph,struct udphdr** udph_p)
{
	struct udphdr* udph;
	if(iph->protocol == IPPROTO_UDP)
	{
		udph = (struct udphdr*)((u8*)iph + iph->ihl*4);
		*udph_p = udph;
		return 0;
	}
	return -1;
}
// SIP包内容标记
#define SIPTAGANDVERSION1 "SIP/2.0"		// 回应, 在第一行开始位置
#define SIPTAGANDVERSION2 " SIP/2.0"	// 请求, 在第一行结束位置
// SIP包结束标记
#define SIPPACKENDTAG "\r\n\r\n"
#define SIPSDPPACKENDTAG "\r\n"

/* 在一行的最后加个\0,\r\n-->\0 然后返回下一行。 */
char* split_line_next(char* l)
{
	char* next = strstr(l,SIPSDPPACKENDTAG);
	if(next == NULL)
		return next;
	next[0] = 0;
	next+=2;
	return next;
}
char** parse_tokens(char* src,int* count)
{
    char* array[100] = {0};
    char** path = NULL;
    int i = 0;
    char* p = NULL;
    char* in = src;
    char seps[] = " ";
    do{
        p = strtok(in,seps);
        if(p)
        {
            array[i] = p;
            i++;
            if(i >=100)
                break;
        }
        in = NULL;
    }while(p);
    path = (char**)malloc(sizeof(char*)*(i+1));
    if(!path)
        return NULL;
    memset(path,0,sizeof(char*)*(i+1));
    memcpy(path,array,sizeof(char*)*i);
    *count = i;
    return path;
}
#if 0
int __parse_line(char* line,char**key,char** v)
{
	
	char *pKey = NULL;
	char *pV = NULL;
	char seps[] = ":";
	static char nullBuf[] = "";
	pKey = strtok(line, seps);
	if(pKey)
	{
    	pV = strtok(NULL, seps);
    	if(pV == NULL)
    	{
    		pV = nullBuf;
    	}
	}
	else
	{	
		return -1;
	}

	pKey = skip_space(pKey);
	pV = skip_space(pV);
	*v = pV;
	*key = pKey;
	return 0;
}
#endif
int parse_sdp_connection_info(char* p, struct sip_pkt* sp)
{
    int count;
    char** media_ele;
    if(NULL == split_line_next(p))
        return -1;
    media_ele = parse_tokens(p,&count);
    if(media_ele != NULL && count >= 3){
        log(" %s %s %s \n",media_ele[0],media_ele[1],media_ele[2]);
       inet_aton(media_ele[2], &sp->rtp_ip );
    }
    FREE(media_ele);
}
int parse_sdp_media_dest(char* p,struct sip_pkt* sp)
{
    //find port号
    // media_type Media_port Medai_protocol fromat format 
    int count;
    char** media_ele;
    if(NULL == split_line_next(p))
        return -1;
    media_ele = parse_tokens(p,&count);
    if(media_ele != NULL && count >= 2){
        log(" %s %s \n",media_ele[0],media_ele[1]);
        sp->rtp_port = atoi(media_ele[1]);
    }
    FREE(media_ele);
    return 0;
}
int parse_msg_body(struct sip_pkt* sp)
{
    //主要是处理 SDP.
    //find "m="
    char* b = sp->sip_msg_body;
    const char* key="m=";
    const char* key2="c=";
    char* p = strstr(b,key);
    if(p != NULL)
    {
        p+=2;
        parse_sdp_media_dest(p,sp);
    }
    
    p = strstr(b,key2);
    if(p != NULL)
    {
        p+=2;
        log("DEBUG here\n");
        parse_sdp_connection_info(p,sp);
    }
    return 0;
}

//return value and length of value
char* __find_msg_hdr_key(char* mh,const char* key,int* v_len)
{
    return find_key_from_line(mh,key,v_len,": ");

}
char* setup_sip_pkt(char* mh,const char* key,char** dest)
{
    int len=0;
    char* v;
    v = __find_msg_hdr_key(mh,key,&len);
    if(v)
        *dest = strndup(v,len);
    return *dest;
}

int parse_msg_header(char* mh,struct sip_pkt* sp)
{
	const char* key = "Content-Length";
	const char* key1 = "Content-Type";
	const char* call_id = "Call-ID";
	
	char* v;
	int len;
	
    v = setup_sip_pkt(mh,call_id,&sp->msg_hdr.call_id);
	if(v == NULL)
	{
	    log_err("I not find the callid \n");
	}
	else
	    log("I get callid %s \n",v);

    v= __find_msg_hdr_key(mh,key,&len);
	if(v != NULL)
	{
	    sp->msg_hdr.content_length = atoi(v);
	    log("%s== %d \n",key,sp->msg_hdr.content_length);
	}
	else
	    log("not find %s failed \n",key);

    v= __find_msg_hdr_key(mh,key1,&len);
    if(v!= NULL)
    {
	    if(!strncmp(v,"application/sdp",len))
        {
            sp->body_sdp = 1;
            log("body have sdp  \n");
        }
    }
    else
        log("not find %s \n",key1);

    return 0;
}

int parse_sip_start_line_request_state(char* l,struct sip_pkt* sp)
{
    const char* key2  = "INVITE ";
    char* p;
    log("start line %s \n",l);
    p = strstr(l,key2);
    if(p)
    {
        sp->state = SS_INVATE;
        return 0;
    }
    p = strstr(l,"ACK ");
     if(p)
    {
        sp->state = SS_ACK;
        return 0;
    } 
    
    p = strstr(l,"BYE ");
     if(p)
    {
        sp->state = SS_BYE;
        return 0;
    }   
    return 0;
}
int parse_sip_start_line_response_state(char* l,struct sip_pkt* sp)
{
    const char* key2  = "100 Trying";
    char* p;
    log("start line %s \n",l);
    p = strstr(l,key2);
    if(p)
    {
        sp->state = SS_TRYING;
        return 0;
    }
    p = strstr(l,"200 OK");
     if(p)
    {
        sp->state = SS_OK;
        return 0;
    } 
    p = strstr(l,"Ringing ");
     if(p)
    {
        sp->state = SS_RINGING;
        return 0;
    }   
    return 0;
}

int pase_sip_start_line(char* l,struct sip_pkt* sp)
{
	const char* sip_key = " SIP/2.0";
	const  char* sip_key1 = "SIP/2.0 ";
//	const char* key2  = "INVITE ";
	//struct session_info* ss = NULL;
	char* p = strstr(l,sip_key);
	if(p != NULL)
	{
	    log("request \n");
		sp->type = 1;  //request;
	}
	else if((p = strstr(l,sip_key1)) != NULL)
	{
	    log("response \n");
	    sp->type = 2;// response
	}
	else
	    return -1;
	    
    if(sp->type == 1 )
       parse_sip_start_line_request_state(l,sp);
   else if(sp->type == 2)
       parse_sip_start_line_response_state(l,sp);
	return 0;
}

void _create_session(struct sip_pkt* spkt_p)
{
    struct session_info* ss = _si_new_session();
    if(ss)
    {
        ss->state = spkt_p->state;
        ss->call_id = strdup(spkt_p->msg_hdr.call_id);
        ss->calling.ip.s_addr = spkt_p->rtp_ip.s_addr;
        ss->calling.port= spkt_p->rtp_port;
        log("I create new session !!!!!!!!! callid %s \n",ss->call_id);
    }
}
void _update_session(struct sip_pkt* spkt_p)
{
    struct session_info* ss;
    if(spkt_p->msg_hdr.call_id)
    {
        log("debug here \n");
        ss = _si_find_session(spkt_p->msg_hdr.call_id);
        log("debug here \n");
        if(ss != NULL)
        {
            log("I find the session (callid %s) \n",ss->call_id);
            ss->state = spkt_p->state;
            if(spkt_p->body_sdp)
            {
                ss->called.ip.s_addr = spkt_p->rtp_ip.s_addr;
                ss->called.port = spkt_p->rtp_port;
                log("I find the session (callid %s) \n",ss->call_id);
                ss->rtp_sniffer_tid = setup_rtp_sniffer(ss);
            }
        }
        else
        {
            log("not find the session (callid %s) \n",spkt_p->msg_hdr.call_id);
        }
    }
    else
    {
        log_err(" this spkt not callid ,so bad \n");
    }
}

void _close_session(struct sip_pkt* spkt_p)
{
    struct session_info* ss;
    if(spkt_p->msg_hdr.call_id)
    {
        ss = _si_find_session(spkt_p->msg_hdr.call_id);
        if(ss != NULL)
        {
            log("I find the session (callid %s) \n",ss->call_id);
            ss->state = spkt_p->state;
            
            log("I find the session (callid %s) \n",ss->call_id);
            close_rtp_sniffer(ss);
        }
        else
        {
            log_err("not find the session (callid %s) \n",spkt_p->msg_hdr.call_id);
        }
        _si_del_session(ss);
    }
    else
    {
        log_err(" this spkt not callid ,so bad \n");
    }

}
void sync_session(struct sip_pkt* spkt_p)
{
    log("pkt state %d \n",spkt_p->state);
    switch(spkt_p->state)
    {
        case SS_INVATE:
        _create_session(spkt_p);
        break;
        case SS_ACK:
        case SS_OK:
        _update_session(spkt_p);
        break;
        case SS_BYE:
        _close_session(spkt_p);
        break;
        case SS_ERROR:
        case SS_TRYING:
        case SS_RINGING:
        default:
        break;
    }
}
/**********************************************************************/
void __free_sip_pkt(struct sip_pkt* spkt_p)
{
    struct sip_msg_header* msg_hdr = &spkt_p->msg_hdr;

    FREE(msg_hdr->contant);
    FREE(msg_hdr->expires);
    FREE(msg_hdr->to);
    FREE(msg_hdr->call_id);
    FREE(msg_hdr->via);
    FREE(msg_hdr->from);
    FREE(msg_hdr->cseq);
    FREE(msg_hdr->user_agent);
    FREE(msg_hdr->Max_forwards);
    FREE(msg_hdr->allow);
    
}
int check_sip( struct udphdr* udph)
{
	//char* sip = (u8*)(udph+1);
	
	struct sip_pkt spkt;
	struct sip_pkt* spkt_p = &spkt;
	char* mesg_header;
	char* mesg_body;
	char* sip = malloc(htons(udph->len));
	if(sip == NULL)
		return -1;
	memcpy(sip,(u8*)(udph+1),htons(udph->len)-8);

	memset(&spkt,0,sizeof(spkt));

    mesg_header = split_line_next(sip);
    if(mesg_header == NULL)
        return -1;

	mesg_body = strstr(mesg_header,SIPPACKENDTAG);
    if(mesg_body == NULL){
        log("sip not messgae body \n");
        
    }
    else{
        mesg_body[2] = 0;
        mesg_body[3] = 0;
        spkt_p->sip_msg_body = mesg_body+strlen(SIPPACKENDTAG);
    }
    //三大块切分完成。
    //下面各处理各的。
    //start-line
	spkt_p->line = sip;
	
    log("[%s:%d]I spkt_p->line %s \n" ,__func__,__LINE__,spkt_p->line);
	pase_sip_start_line(spkt_p->line,spkt_p);
	
	//message header
	
	if((mesg_header[0] != '\r')&&(mesg_header[1] != '\n'))
	{
		parse_msg_header(mesg_header,spkt_p);
	}
	if (spkt_p->sip_msg_body)
	    parse_msg_body(spkt_p);


	sync_session(spkt_p);
	__free_sip_pkt(spkt_p);
	return 0;
	
}


/***********************************************************************************/
static void sniffer_handle_sip(u_char * user, const struct pcap_pkthdr * packet_header, const u_char * packet_content)
{
    int ret = 0;
    
	const struct pcap_pkthdr* phdr = packet_header;
	struct iphdr* iph = NULL;
	struct udphdr* udph = NULL;
	
    //printf("[%s:%d]I get one packet! \n" ,__func__,__LINE__);
	ret = check_iphdr(phdr,packet_content,&iph);
	if(ret != 0)
		goto error;
	
    //printf("[%s:%d]I get one packet! \n" ,__func__,__LINE__);
	if(0 != check_udp(iph,&udph))	
		goto error;
	
	
		check_sip(udph);
	
error:
	return;
}

/*
这里有两个抓包的过程，一是只抓 sip, 另一个抓RTP
这里就需要两个线程。一个线程是常驻，用于抓SIP 
另一个线程是动态的，用于抓RTP.

*/
int sniffer_loop_sip( pcap_t *p)
{

	

	 pcap_loop( p,-1,sniffer_handle_sip,(u_char*)NULL);
	 return 0;
}

void* sniffer_sip_loop(void* arg)
{

	char filter[200] = {0};
    printf("%s:%d \n",__func__,__LINE__);
    pcap_t* pd=0;
    #if 1
	//pd = open_pcap_file("enp0s3",65535,1,0);
	pd = open_pcap_file("eth0",65535,1,0);
	if(pd == NULL)
	{

		log("open_pcap_file failed ! \n");
		exit(1);
	}
	#if 1
	sprintf(filter,"udp and host %s and port %d ",
	inet_ntoa(g_config.call.ip),
	g_config.call.port);
	#else
	sprintf(filter,"icmp");
	#endif
	sniffer_setfilter(pd,filter);
	
#endif
    
    log("sniffer_loop_sip  \n");

	while(1)
	{
		sniffer_loop_sip(pd);
	}
}


pthread_t __sniffer_sip_start(void)
{
	pthread_t tid;
	
   // printf("%s:%d \n",__func__,__LINE__);
	if(pthread_create(&tid,NULL,sniffer_sip_loop,NULL))
	{
		log("create msg_engine_start sniffer_sip_loop failed\n");
		return -1;
	}

   // printf("%s:%d \n",__func__,__LINE__);

	return tid;

}


int __sniffer_init()
{
    INIT_LIST_HEAD(&sip_ctx.si_head);
	return 0;
}

pthread_t sniffer_sip_start(void)
{
	pthread_t tid;
	if(__sniffer_init() != 0)
	{
		log("sniffer failed ,so program exit!\n");
		exit(1);
	}
	tid = __sniffer_sip_start();
    log("%s:%d tid %d\n",__func__,__LINE__,tid);
	return tid;

}

