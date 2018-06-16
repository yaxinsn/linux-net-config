
/******************************************
说明:
本程序，会启动一个专用于抓RTP报文的线程。
由sniffer_sip来启动或是关闭线程。
把抓到的RTP报文按一定的要求，然后保存成文件。
***************************************/
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
//#include "upload.h"


#include "mixer.h"

#include "curl_upload.h"

#include "phone_session.h"

extern struct config_st g_config;
struct rtp_session_info
{
    
    struct list_head node;
    pthread_t   thread_id;
    pcap_t*     pd;
   
    int     call_dir; /// is same the ss mode.
 
    struct  person calling;  //sip msg header From
    struct  person called;   //sip msg header to
    //FILE*   save_all_fp; 
    char calling_name[256];
    
    char called_name[256];
    FILE*   save_calling_fp;
    FILE*   save_called_fp;
    u8     rtp_type;

    time_t start_time_stamp;

    time_t stop_time_stamp;
    //struct session_info* session;
    struct tm ring_time; 
    struct tm end_time;

/* 2018-6-10 */
    struct mixer stMix;
    int calling_pkt_count;
    int called_pkt_count;
    int mix_count;

    g722_decode_state_t* g722dst;
    FILE* save_mix_fp; 

    FILE* save_calling_linear_fp; 
    FILE* save_called_linear_fp; 
    
    char calling_name_linear[256];
    
    char called_name_linear[256];
};


struct rtp_ctx_t
{
    pthread_mutex_t head_lock;  //sync
    
    struct list_head rtp_head;

};
struct rtp_ctx_t rtp_ctx;


struct rtp_session_info* _rtp_new_session()
{
    struct rtp_session_info* rs = NULL;
	
    rs = (struct rtp_session_info*)malloc(sizeof(struct rtp_session_info));
    if ( rs == NULL)
        return NULL;
    bzero(rs,sizeof(struct rtp_session_info)); 
    rs->rtp_type = 1; ///  1 is reserved;
    pthread_mutex_lock(&rtp_ctx.head_lock);
    list_add(&rs->node,&rtp_ctx.rtp_head);
    pthread_mutex_unlock(&rtp_ctx.head_lock);
    return rs;
}
void _rtp_del_session(struct rtp_session_info* si)
{
   
    pthread_mutex_lock(&rtp_ctx.head_lock);
    g722_decode_release(si->g722dst);
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

enum RTP_TYPE
{
    RTP_TYPE_PCMU = 0,//711
    RTP_TYPE_PCMU_GSM = 3,
    RTP_TYPE_PCMU_G723,
    RTP_TYPE_PCMU_G722 = 9,
    RTP_TYPE_PCMU_G729 = 18,
    
};

struct rtp_type_str
{
    enum RTP_TYPE type;
    char* type_str;
};
struct rtp_type_str g_rtp_file_perfix[] = 
{
    {RTP_TYPE_PCMU,"g711"},
    {RTP_TYPE_PCMU_GSM,"gsm"},
    {RTP_TYPE_PCMU_G723,"g723"},
    {RTP_TYPE_PCMU_G722,"g722"},
    {RTP_TYPE_PCMU_G729,"g729"},
};

#define RTP_TYPE_PCMU  0

#if defined(__LITTLE_ENDIAN_BITFIELD)
struct rttphdr{
    
    u8 source_id_count:4 ,
       extension:1 ,
       padding:1,
       version:2;
    u8 type:7,
       marker:1;
    u16 sequence_number;
    u32 timestamp;
    u32 synchronization_source_id;
}__attribute__  ((__packed__));

#elif defined (__BIG_ENDIAN_BITFIELD)
struct rttphdr{
    u8 version:2,
       padding:1,
       extension:1,
       source_id_count:4;
       
    u8 marker:1,
       type:7;
    u16 sequence_number;
    u32 timestamp;
    u32 synchronization_source_id;
}__attribute__  ((__packed__));

#else
#error  "Please fix <asm/byteorder.h>"
#endif


static int save_rtp_frame(FILE* fp,void* buffer,int len)
{
    if(fp)
        return fwrite(buffer,len,1,fp);
    return -1;
}
u8* rtp_g722_decode(g722_decode_state_t* g722dst,char *src, int src_len, int* len) 
{
	//*len = 0;

	
	if(NULL==src) return NULL;

	int samples = src_len; 
	const uint8_t* uc_src=(const uint8_t*)src;


	*len = samples*2;
	char* char_dst=malloc( sizeof(char)*(samples*2));
	if(char_dst == NULL)
	{
	    log_err("malloc err!\n");
	    return NULL;
	}
	memset(char_dst, 0, *len);
	int16_t *dst=(int16_t*)char_dst;


	*len = g722_decode((g722_decode_state_t*)g722dst, dst, uc_src, src_len);


	// g722_decode返回的长度应该是收int16_t类型计算的, 因此要*sizeof(int16_t) _ 相当于*2
	*len *= 2;

	return char_dst;
}

static int session_talking_pkt_dec722
(struct rtp_session_info* rs,u8* payload, int payload_len,
   u8 rty_type,FILE* fp)
{
    int dest_g722_len;
    u8* dest_buf;
    int mix_len;
    bool ret;
    
    if(rty_type == RTP_TYPE_PCMU_G722)
    {
    
        dest_buf = rtp_g722_decode(rs->g722dst,payload,payload_len,&dest_g722_len);
        if(dest_buf){
            fwrite(dest_buf,dest_g722_len,1,fp);
            free(dest_buf);
            
        }
        else
        {
            log_err("g722 decode error! \n");
            return -1;
        }
    }
    else
    {
        log_err("this pkt not a g722, I can't decode it \n");
        return -1;
    }
   
     
     return 0;
}
static void session_talking(struct iphdr* iph,struct udphdr* udph,
    struct rtp_session_info* rs)
{
    u8* rtp_pkt = (u8*)(udph+1);
    int rtp_len = ntohs(udph->len)-8;
    u8* rtp_payload = NULL;
    struct rttphdr* rtp_hdr;
    rtp_hdr = (struct rttphdr*)rtp_pkt;
    
   // struct phone_msg* msg;
    rtp_payload = rtp_pkt+sizeof(struct rttphdr);
    if(rs->rtp_type == 1)
    {
        rs->rtp_type = rtp_hdr->type;
        
        log("get a rty_type firstly %d \n",rtp_hdr->type);
    }
    if(rs->rtp_type != rtp_hdr->type)
    {
        log_err("rtp pkt type %d , session rtp type %d\n",rtp_hdr->type,rs->rtp_type);
        
    }
    if(iph->saddr == rs->calling.ip.s_addr)
    {
       
        rs->calling_pkt_count++;
        session_talking_pkt_dec722(rs,rtp_payload,
            rtp_len-sizeof(struct rttphdr),rs->rtp_type,
            rs->save_calling_linear_fp);
        //save_rtp_frame(rs->save_calling_fp,rtp_payload,rtp_len-sizeof(struct rttphdr));
    }

    if(iph->saddr == rs->called.ip.s_addr)
    {
   
        rs->called_pkt_count++;
        
        session_talking_pkt_dec722(rs,rtp_payload,
            rtp_len-sizeof(struct rttphdr),rs->rtp_type,
            rs->save_called_linear_fp);
  
       // save_rtp_frame(rs->save_called_fp,rtp_payload,rtp_len-sizeof(struct rttphdr));
    }

    //save_rtp_frame(rs->save_all_fp,rttp_payload,rtp_len-sizeof(struct rttphdr));
}

/****************************************************/
char* get_rtp_type(u8 type)
{
    int i;
    for(i = 0;i<sizeof(g_rtp_file_perfix)/sizeof(struct rtp_type_str);i++)
    {
        if(type == g_rtp_file_perfix[i].type)
            return g_rtp_file_perfix[i].type_str;
    }
    return "NA";
}
#define READ_BUF_SIZE 1024
int mix_the_linear_file(struct rtp_session_info* n)
{
    char save_file_name[256] = {0};
    int fp_calling;
    int fp_called;
    FILE* dest_fp;
    u8 calling_buf[READ_BUF_SIZE];
    u8 called_buf[READ_BUF_SIZE];
    int reta;
    int retb;
    int mix_len = 0;
    int break_flag = 0;
    
    char ring_time[64] =  {0};

    fp_calling = open(n->calling_name_linear,O_RDONLY);
    fp_called = open(n->called_name_linear,O_RDONLY);

    log("DEBUG fp_calling %d fp_called %d \n",fp_calling,fp_called);

        
    strftime(ring_time,256,"%Y-%m-%d-%H-%M-%S",&n->ring_time);
                
    sprintf(save_file_name,"/tmp/from_%s_to_%s_startTime_%s.mix",
        n->calling.number,n->called.number,ring_time);
        
    log("save file name %s \n",save_file_name);    
    
    dest_fp = fopen(save_file_name,"w");

    while(1)
    {
        memset(calling_buf,0,sizeof(calling_buf));
        memset(called_buf,0,sizeof(calling_buf));
        
        reta = read(fp_calling,calling_buf,READ_BUF_SIZE);
        retb = read(fp_called,called_buf,READ_BUF_SIZE);
        if(reta  <= 0 )//read end
        {
            log_err("calling file read errro , reta = %d \n",reta);
            break_flag |= 1;
        }
        else if(reta > 0)
        {
            mix(&n->stMix,calling_buf,reta,&mix_len);
            log("mix_len %d \n", mix_len);
        }
        
        if(retb  <= 0 )//read end
        {
            break_flag |= 2;
            log_err("called file read errro , reta = %d \n",retb);
        }
        else if(reta > 0)
        {
            mix(&n->stMix,called_buf,retb,&mix_len);
            log("mix_len %d \n", mix_len);
        }
        fwrite(n->stMix.data,mix_len,1,dest_fp);
        if(break_flag == 3){
            
             log("debug break it\n");
            break;
        }
    }
    fclose(dest_fp);
    close(fp_calling);
    log("debug \n");
    close(fp_called);
    return 0;
}

int cul_rtp_end_time(struct rtp_session_info* n)
{

    u32 duration;

    time_t a;
    struct tm* tt;
    a = mktime(&n->ring_time);
    
    duration = n->stop_time_stamp - n->start_time_stamp;

    a += duration;

    tt = localtime(&a);
    memcpy(&n->end_time,tt,sizeof(struct tm));
    return 0;
}
int upload_the_mix_file( struct rtp_session_info* n)
{
    int ret;
    struct upload_file_info ufi;
    char time_str[256]={0};
    struct config_st* c = &g_config;
    
    char save_file_name[256]={0};
    
    char ring_time[256]={0};
    
    //memcpy(ufi.box_id,g_config.eth0_mac,6);
    
    strncpy(ufi.call_caller_number,n->calling.number,sizeof(ufi.call_caller_number));
    
    strncpy(ufi.call_callee_number,n->called.number,sizeof(ufi.call_callee_number));
 
    if(n->call_dir == SS_MODE_CALLING){
        sprintf(ufi.call_direction,"%d",0);
        
        strncpy(ufi.call_local_number,n->calling.number,sizeof(ufi.call_local_number));
    }
    else
    {
        sprintf(ufi.call_direction,"%d",1);
        strncpy(ufi.call_local_number,n->called.number,sizeof(ufi.call_local_number));
    }

    strftime(time_str,256,"%Y-%m-%d %H:%M:%S",&n->ring_time);
    strncpy(ufi.call_begin_time,time_str,sizeof(ufi.call_begin_time));
    
    memset(time_str,0,sizeof(time_str));
    strftime(time_str,256,"%Y-%m-%d %H:%M:%S",&n->end_time);
    strncpy(ufi.call_end_time,time_str,sizeof(ufi.call_end_time));

    strftime(ring_time,256,"%Y-%m-%d-%H-%M-%S",&n->ring_time);     
    sprintf(ufi.file_name,"from_%s_to_%s_startTime_%s.mix",
            n->calling.number,n->called.number,ring_time);
            
    sprintf(ufi.box_id,"%02X:%02X:%02X:%02X:%02X:%02X:%02X",
        c->eth0_mac[0],c->eth0_mac[1],c->eth0_mac[2],
	    c->eth0_mac[3],c->eth0_mac[4],c->eth0_mac[5]);
    ret = upload_mix_file(c->upload_http_url,&ufi);
    return ret;
}

static void sighandler(int s)
{
        int retval = 3;
        struct rtp_session_info* n;

        
        n = _rtp_find_session(pthread_self());
        if(n)
        {
        log("I  find rtp session -------n->rtp_typ %d-----\n",n->rtp_type);
            time(&n->stop_time_stamp);
                cul_rtp_end_time(n);
            
            pcap_close(n->pd);
            if(n->save_called_fp)
                fclose(n->save_called_fp);
            if(n->save_calling_fp)
                fclose(n->save_calling_fp);

            if(n->save_mix_fp)
                fclose(n->save_mix_fp);
            if(n->save_calling_linear_fp)
                fclose(n->save_calling_linear_fp);
            
            if(n->save_called_linear_fp)
                fclose(n->save_called_linear_fp);

            mix_the_linear_file(n);    

            upload_the_mix_file(n);
           
            _rtp_del_session(n);
        }
        else
        {
            log_err("not find rtp session \n");
        }
        log(" %u thread quit \n", (unsigned long)pthread_self());
        pthread_exit(&retval);
        
}

#if 0
static void sighandler(int s)
{
        int retval = 3;
        char* file_perfix;
        struct rtp_session_info* n;
        char new_name[300];
        char ring_time[300];
        int t=0;
        u32 duration;
        char calledip_str[30]={0};
        char callingip_str[30]={0};
        
        n = _rtp_find_session(pthread_self());
        if(n)
        {
        log("I  find rtp session -------n->rtp_typ %d-----\n",n->rtp_type);
            time(&n->stop_time_stamp);
            duration = n->stop_time_stamp - n->start_time_stamp;
            
                strftime(ring_time,256,"%Y-%m-%d_%H-%M-%S",&n->ring_time);
                sprintf(calledip_str, "%s",inet_ntoa(n->called.ip));
                sprintf(callingip_str, "%s",inet_ntoa(n->calling.ip));
                
            
            pcap_close(n->pd);
            if(n->save_called_fp)
                fclose(n->save_called_fp);
            if(n->save_calling_fp)
                fclose(n->save_calling_fp);
            file_perfix = get_rtp_type(n->rtp_type);

            if(n->save_mix_fp)
                fclose(n->save_mix_fp);

            if(n->save_calling_linear_fp)
                fclose(n->save_calling_linear_fp);
            
            if(n->save_called_linear_fp)
                fclose(n->save_called_linear_fp);

            mix_the_linear_file(n);    
            memset(new_name,0,sizeof(new_name));
            sprintf(new_name,"/tmp/from_%s_startTime_%s_duration_%d.%s",
                callingip_str,ring_time,duration, file_perfix);
            rename(n->called_name,new_name); 
            
            memset(new_name,0,sizeof(new_name));
            sprintf(new_name,"/tmp/to_%s_startTime_%s_duration_%d.%s",
                calledip_str,ring_time,duration, file_perfix);
            rename(n->calling_name,new_name);
            
           
            _rtp_del_session(n);
        }
        else
        {
            log_err("not find rtp session \n");
        }
        log(" %u thread quit \n", (unsigned long)pthread_self());
        pthread_exit(&retval);
        
}
#endif
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
    
    //time(&ss->stop_time_stamp);
    struct rtp_session_info* n;
    if(ss->rtp_sniffer_tid)
    {
    
        log(" I %ul kill %ul thread(rtp) \n",(unsigned long)pthread_self()
                ,(unsigned long)ss->rtp_sniffer_tid);
        n = _rtp_find_session(ss->rtp_sniffer_tid);
        if(n)
        {
            log("sync time;\n");
            memcpy(&n->ring_time,&ss->ring_time,sizeof(ss->ring_time));
        }

                
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
	if(ret != 0){
	    log_err("ip header error\n");
		goto error;
    }
	
	if(0 != check_udp(iph,&udph))	{
	    log_err("udp error\n");
		goto error;
    }
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
	return NULL;
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
	//char mix_file[256]={0};
	//char file_name2[256]={0};
	//char file_name3[256]={0};
	char file_name2_linear[256]={0};
	char file_name3_linear[256]={0};
	struct rtp_session_info* rs;
	int t= 0;

    time_t a;
    time(&a);
    //ss->start_time_stamp = a;
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
	
	log("sniffer calling %s:%d phone_number %s \n",
	    inet_ntoa(ss->calling.ip),ss->calling.port,ss->calling.number);
	log("sniffer called  %s:%d phone_number %s \n",
	    inet_ntoa(ss->called.ip),ss->called.port,ss->called.number);

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
    //rs->session = ss;
    
    ss->start_time_stamp = a;
    rs->start_time_stamp = a;
    rs->call_dir = ss->mode;
    memcpy(&rs->called,&ss->called,sizeof(ss->called));
    memcpy(&rs->calling,&ss->calling,sizeof(ss->calling));
    log("DEBUG here\n");
    memcpy(&rs->called,&ss->called,sizeof(struct  person));
    memcpy(&rs->calling,&ss->calling,sizeof(struct  person));
    rs->pd = pd;
    
    rs->g722dst = (void*)g722_decode_init((g722_decode_state_t*) rs->g722dst, G722BITSRATE, 1 /* 表明采用8000采样 */);

#if 0
    t += sprintf(mix_file,"/tmp/%s_to_",inet_ntoa(ss->calling.ip));
    t += sprintf(mix_file+t,"%s_",inet_ntoa(ss->called.ip));
    t += sprintf(mix_file+t,"%lu.mixed_linear",a);
    
    log("DEBUG here MIX file  %s\n",mix_file);
    rs->save_mix_fp = fopen(mix_file,"w");
   
    t=0;
    t += sprintf(file_name2,"/tmp/to_%s_",inet_ntoa(ss->called.ip));
    t += sprintf(file_name2+t,"%lu.pkt",a);
    rs->save_called_fp = fopen(file_name2,"w");
    strcpy(rs->called_name,file_name2);
    
    t=0;
    t += sprintf(file_name3,"/tmp/from_%s_",inet_ntoa(ss->calling.ip));
    t += sprintf(file_name3+t,"%lu.pkt",a);
    rs->save_calling_fp = fopen(file_name3,"w");
    strcpy(rs->calling_name,file_name3);

    
#endif
    t=0;
    t += sprintf(file_name2_linear,"/tmp/to_%s_",inet_ntoa(ss->called.ip));
    t += sprintf(file_name2_linear+t,"%lu.linear",a);
    rs->save_called_linear_fp = fopen(file_name2_linear,"w");
    strcpy(rs->called_name_linear,file_name2_linear);
    
    t=0;
    t += sprintf(file_name3_linear,"/tmp/from_%s_",inet_ntoa(ss->calling.ip));
    t += sprintf(file_name3_linear+t,"%lu.linear",a);
    rs->save_calling_linear_fp = fopen(file_name3_linear,"w");
    strcpy(rs->calling_name_linear,file_name3_linear);


    
    log("DEBUG here\n");

	//session_up();
	
	if(pthread_create(&tid,NULL,sniffer_rtp_loop1,rs))
	{
		log("create msg_engine_start sniffer_sip_loop failed\n");
		return -1;
	}
	rs->thread_id = tid;
	log("rs thread_id %u \n",rs->thread_id);
	
    pthread_detach(tid);//线程与sip线程分离。

	return tid;

}

void rtp_sniffer_init(void)
{

    INIT_LIST_HEAD(&rtp_ctx.rtp_head);

}
