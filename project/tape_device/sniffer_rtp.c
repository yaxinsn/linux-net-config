
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

#include "phone_session.h"

struct rtp_session_info
{
    
    struct list_head node;
    pthread_t   thread_id;
    pcap_t*     pd;
    
    struct  person calling;  //sip msg header From
    struct  person called;   //sip msg header to
    FILE*   save_all_fp; 
    FILE*   save_calling_fp;
    FILE*   save_called_fp;
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
    bzero(rs,sizeof(struct rtp_session_info));  
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
//这个函数也应该没作用了。
#if 0
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
#endif
/* 向upload发一个ring down的报文。应该是没什么作用了。 */
static void session_down()
{
    char buf[256] = {0};
    struct phone_msg* msg;
    int len;
    int ret;
    msg = ( struct phone_msg*)buf;
    msg->event = RING_DOWN;
    len = sizeof(struct phone_msg);
#if 0
    ret = uploader_push_msg(msg, len);
    if(ret != 0)
        log_err("uploader_push_msg (RING_DOWN) failed,ret %d \n",ret);
#endif
}

struct rttphdr{
    u8 version;
    u8 type;
    u16 sequence_number;
    u16 extended_sequence_number;
    u32 timestamp;
    u32 synchronization_source_id;
}__attribute__  ((__packed__));
static int save_rtp_frame(FILE* fp,void* buffer,int len)
{
    if(fp)
        return fwrite(buffer,len,1,fp);
    return -1;
}
static void session_talking(struct iphdr* iph,struct udphdr* udph,struct rtp_session_info* rs)
{
    char buf[2000] = {0};

    u8* rtp_pkt = (u8*)(udph+1);
    int rtp_len = ntohs(udph->len)-8;
    u8* rttp_payload = NULL;
   // struct phone_msg* msg;
    rttp_payload = rtp_pkt+sizeof(struct rttphdr);
    
    log("DEBUG here");
    if(iph->saddr == rs->calling.ip.s_addr)
    {
        save_rtp_frame(rs->save_calling_fp,rttp_payload,rtp_len-sizeof(struct rttphdr));
    }

    if(iph->saddr == rs->called.ip.s_addr)
    {
        save_rtp_frame(rs->save_called_fp,rttp_payload,rtp_len-sizeof(struct rttphdr));
    }

    save_rtp_frame(rs->save_all_fp,rttp_payload,rtp_len-sizeof(struct rttphdr));
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
            if(n->save_called_fp)
                fclose(n->save_called_fp);
            if(n->save_calling_fp)
                fclose(n->save_calling_fp);
            if(n->save_all_fp)
                fclose(n->save_all_fp);
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

    //session_down();
    if(ss->rtp_sniffer_tid)
    {
    
        log(" I %ul kill %ul thread(rtp) \n",(unsigned long)pthread_self()
                ,(unsigned long)ss->rtp_sniffer_tid);
        thread_kill(ss->rtp_sniffer_tid);
    }
}

void handle_rtp(struct iphdr* iph,struct udphdr* udph,void* arg)
{


	struct rtp_session_info* rs = arg;

    

    //struct session_info* ss = (struct session_info*)arg;
    session_talking( iph,udph,rs);
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

	struct rtp_session_info* rs = arg;

    pcap_t *pd =rs->pd;

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
	sprintf(filter,"udp and host %s and port %d ",
	    inet_ntoa(ss->calling.ip),
	    ss->calling.port);

	if(sniffer_setfilter(pd,filter) <0){
	    log("rtp sniffer set filter failed!\n");
	    exit(1);
	}
    return pd;
}
pthread_t setup_rtp_sniffer(struct session_info* ss)
{
	pthread_t tid;
	pcap_t* pd;
	char file_name[256]={0};
	char file_name2[256]={0};
	char file_name3[256]={0};
	struct rtp_session_info* rs;
	int t= 0;

    time_t a;
    time(&a);

	if (ss->mode == SS_MODE_CALLED){
	    log("this session is called (slave) \n");
	}
	else if(ss->mode == SS_MODE_CALLING){
	    log("this session is calling(master)\n");
    }
    else
    {
        log_err("this session is bad ession.!!!!");
    }
	 
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
    log("DEBUG here\n");
    memcpy(&rs->called,&ss->called,sizeof(struct  person));
    memcpy(&rs->calling,&ss->calling,sizeof(struct  person));
    rs->pd = pd;
    t += sprintf(file_name,"./%s_to_",inet_ntoa(ss->calling.ip));
    t += sprintf(file_name+t,"%s_",inet_ntoa(ss->called.ip));
   t += sprintf(file_name+t,"%u",a);
    
    log("DEBUG here file_name %s\n",file_name);
    rs->save_all_fp = fopen(file_name,"w");


    t=0;
    t += sprintf(file_name2,"./%s_",inet_ntoa(ss->called.ip));
    t += sprintf(file_name2+t,"%u",a);
    rs->save_called_fp = fopen(file_name2,"w");
    
    t=0;
    t += sprintf(file_name3,"./%s_",inet_ntoa(ss->calling.ip));
    t += sprintf(file_name3+t,"%u",a);
    rs->save_calling_fp = fopen(file_name3,"w");
    
    log("DEBUG here");

	//session_up();
	
	if(pthread_create(&tid,NULL,sniffer_rtp_loop1,rs))
	{
		log("create msg_engine_start sniffer_sip_loop failed\n");
		return -1;
	}
	rs->thread_id = tid;
	
    pthread_detach(tid);//线程与sip线程分离。

	return tid;

}

void rtp_sniffer_init(void)
{

    INIT_LIST_HEAD(&rtp_ctx.rtp_head);

}
