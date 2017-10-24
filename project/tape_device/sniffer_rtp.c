
/*
说明:
本程序，会启动一个专用于抓RTP报文的线程。
由sniffer_sip来启动或是关闭线程。
把抓到的RTP报文按一定的要求，转给upload
*/
#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>


#include <signal.h>

#include "log.h"

#include "sniffer_rtp.h"
#include "upload.h"

struct rtp_session_info
{
    
    struct list_head node;
    pthread_t   thread_id;
    pcap_t*     pd;
};


struct rtp_ctx_t
{
    pthread_mutex_t head_lock;  //sync
    
    struct list_head rtp_head;

};
struct rtp_ctx_t rtp_ctx;


struct rtp_session_info* _rtp_new_session()
{
    struct rtp_session_info* rs =NULL;
    rs = (struct session_info*)malloc(sizeof(struct rtp_session_info));
    if ( rs == NULL)
        return NULL;
        
    pthread_mutex_lock(&rtp_ctx.head_lock);
    list_add(&rs->node,&rtp_ctx.rtp_head);
    pthread_mutex_unlock(&rtp_ctx.head_lock);
    return rs;
}
void _rtp_del_session(struct rtp_session_info* si)
{
   
    pthread_mutex_lock(&rtp_ctx.head_lock);

    list_del(&si->node);
    
    pthread_mutex_unlock(&rtp_ctx.head_lock);
    FREE(si);
    return;
}
struct rtp_session_info* _rtp_find_session(pthread_t   thread_id)
{
    struct rtp_session_info* p;
    struct rtp_session_info* n;
    struct list_head* rtp_head;
    rtp_head = &rtp_ctx.rtp_head;
    
    list_for_each_entry_safe(p,n,rtp_head,node)
    {

        if(thread_id == p->thread_id)
        {
            return p;
        }
    }
    return NULL;
}

/****************************/
static void session_up()
{
    char buf[256] = {0};
    struct phone_msg* msg;
    int len;
    int ret;
    msg = ( struct phone_msg*)buf;
    msg->event = RING_UP;
    len = sizeof(struct phone_msg);
    ret = uploader_push_msg(msg, len);
    if(ret != 0)
        log_err("uploader_push_msg failed,ret %d \n",ret);
    
}
static void session_down()
{
    char buf[256] = {0};
    struct phone_msg* msg;
    int len;
    int ret;
    msg = ( struct phone_msg*)buf;
    msg->event = RING_DOWN;
    len = sizeof(struct phone_msg);
    ret = uploader_push_msg(msg, len);
    if(ret != 0)
        log_err("uploader_push_msg (RING_DOWN) failed,ret %d \n",ret);
    
}
static void session_talking(struct iphdr* iph,struct udphdr* udph)
{
    char buf[2000] = {0};

    u8* rtp_pkt = (u8*)(udph+1);
    int rtp_len = ntohs(udph->len);
    
    struct phone_msg* msg;
    
    struct talking_mesg* tm;
    int len;
    int ret;
    msg = ( struct phone_msg*)buf;
    msg->event = TALKING;
    len = sizeof(struct phone_msg);

    len = rtp_len + sizeof(struct phone_msg) +sizeof( struct talking_mesg);
    if(len > sizeof(buf))
        log_err("total len %d > 2000 \n",len);

    
    tm = (struct talking_mesg*)msg->data;

    tm->phone_sender_ip = iph->saddr; /* 本来就是网络序 */
    tm->phone_sender_port = udph->source;
    tm->phone_rcv_ip = iph->daddr;
    tm->phone_rcv_port = udph->dest;
    tm->lenth = rtp_len;
    
    memcpy(tm->data,rtp_pkt,rtp_len);
    
    
    ret = uploader_push_msg(msg, len);
    if(ret != 0)
        log_err("uploader_push_msg (TALKING) failed,ret %d \n",ret);
    
}

/****************************************************/
static void sighandler(int s)
{
        int retval = 3;
        struct rtp_session_info* n;
        n = _rtp_find_session(pthread_self());
        if(n)
        {
            pcap_close(n->pd);
            _rtp_del_session(n);
        }    
        log(" %ul thread quit \n", (unsigned long)pthread_self());
        pthread_exit(&retval);
        
}

int thread_kill(pthread_t thread_id)
{
        int kill_ret = pthread_kill(thread_id,SIGQUIT);
            //log("%s:%d ret %d \n",__func__,__LINE__,kill_ret);
        if(kill_ret == ESRCH)
        {
                log("the tid is not exist\n");
        }
        else if(kill_ret == EINVAL)
        {
                log("unvalide signal\n");
        }
        else
        {
            log("the tid is exist\n");
        }
        return 0;
}

void close_rtp_sniffer(struct session_info* ss)
{

    session_down();
    if(ss->rtp_sniffer_tid)
    {
    
        log(" I %ul kill %ul thread(rtp) \n",(unsigned long)pthread_self()
                ,(unsigned long)ss->rtp_sniffer_tid);
        thread_kill(ss->rtp_sniffer_tid);
    }
}

void handle_rtp(struct iphdr* iph,struct udphdr* udph,void* arg)
{
    //struct session_info* ss = (struct session_info*)arg;
    session_talking( iph,udph);
}
static void sniffer_handle_rtp(u_char * user, const struct pcap_pkthdr * packet_header, const u_char * packet_content)
{

	int ret = 0;
    
	const struct pcap_pkthdr* phdr = packet_header;
	struct iphdr* iph = NULL;
	struct udphdr* udph = NULL;
	
	ret = check_iphdr(phdr,packet_content,&iph);
	if(ret != 0)
		goto error;
	
	if(0 != check_udp(iph,&udph))	
		goto error;

	handle_rtp(iph,udph,(void*) user);
error:
	return;
}

static int sniffer_rtp_loop2( pcap_t *p,void* arg)
{
	 pcap_loop( p,-1,sniffer_handle_rtp,(u_char*)arg);
	 return 0;
}

static void* sniffer_rtp_loop1(void* arg)
{
    pcap_t *pd =arg;

	while(1)
	{
		sniffer_rtp_loop2(pd,arg);
	}
}

static pcap_t* init_sniffer_rtp(struct session_info* ss)
{
	char filter[200] = {0};
	
    pcap_t* pd=0;
    signal(SIGQUIT, sighandler);
	//pd = open_pcap_file("enp0s3",65535,1,0);
	pd = open_pcap_file("eth0",65535,1,0);
	if(pd == NULL)
	{
		log("open_pcap_file failed ! \n");
		return NULL;
	}
#if 1
	
	sprintf(filter,"udp and host %s and port %d ",
	    inet_ntoa(ss->calling.ip),
	    ss->calling.port);
#else
	sprintf(filter,"icmp");
#endif
	sniffer_setfilter(pd,filter);
    return pd;
}
pthread_t setup_rtp_sniffer(struct session_info* ss)
{
	pthread_t tid;
	pcap_t* pd;
	struct rtp_session_info* rs;
	log("sniffer rtp info: \n");
	
	log("sniffer calling %s:%d \n",inet_ntoa(ss->calling.ip),ss->calling.port);
	log("sniffer called  %s:%d \n",inet_ntoa(ss->called.ip),ss->called.port);

    pd =  init_sniffer_rtp(ss);
    if(pd == NULL)
    {
        return 0;//bad
    }
    rs = _rtp_new_session();
    if(rs == NULL)
    {
        log_err("rtp_new_session failed\n");
        
    }
    
	session_up();
	
	if(pthread_create(&tid,NULL,sniffer_rtp_loop1,pd))
	{
		log("create msg_engine_start sniffer_sip_loop failed\n");
		return -1;
	}
	if(rs){
	    rs->pd = pd;
	    rs->thread_id = tid;
	}
    pthread_detach(tid);

	return tid;

}

void rtp_sniffer_init(void)
{

    INIT_LIST_HEAD(&rtp_ctx.rtp_head);

}
