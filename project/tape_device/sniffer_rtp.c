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

static void sighandler(int s)
{
        int retval = 3;
        log(" %ul thread quit \n",(unsigned long)pthread_self());
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

    if(ss->rtp_sniffer_tid)
    {
    
        log(" I %ul kill %ul thread(rtp) \n",(unsigned long)pthread_self()
                ,(unsigned long)ss->rtp_sniffer_tid);
        thread_kill(ss->rtp_sniffer_tid);
    }
}


static void sniffer_handle_rtp(u_char * user, const struct pcap_pkthdr * packet_header, const u_char * packet_content)
{
    int ret = 0;
    
	const struct pcap_pkthdr* phdr = packet_header;
	struct iphdr* iph = NULL;
	struct udphdr* udph = NULL;
	
    printf("[%s:%d] rtp::::: I get one packet! \n" ,__func__,__LINE__);

	
error:
	return;
}

static int sniffer_rtp_loop2( pcap_t *p)
{

	

	 pcap_loop( p,-1,sniffer_handle_rtp,(u_char*)NULL);
	 return 0;
}

static void* sniffer_rtp_loop1(void* arg)
{

	char filter[200] = {0};
	struct session_info* ss = arg;
	
    pcap_t* pd=0;
    signal(SIGQUIT, sighandler);
	//pd = open_pcap_file("enp0s3",65535,1,0);
	pd = open_pcap_file("eth0",65535,1,0);
	if(pd == NULL)
	{

		log("open_pcap_file failed ! \n");
		exit(1);
	}
	#if 1
	
	sprintf(filter,"udp and host %s and port %d ",
	    inet_ntoa(ss->calling.ip),
	    ss->calling.port);
	#else
	sprintf(filter,"icmp");
	#endif
	sniffer_setfilter(pd,filter);
	
    

	while(1)
	{
		sniffer_rtp_loop2(pd);
	}
}





pthread_t setup_rtp_sniffer(struct session_info* ss)
{
	pthread_t tid;
	log("sniffer rtp info: \n");
	
	log("sniffer calling %s:%d \n",inet_ntoa(ss->calling.ip),ss->calling.port);
	log("sniffer called  %s:%d \n",inet_ntoa(ss->called.ip),ss->called.port);
	
	if(pthread_create(&tid,NULL,sniffer_rtp_loop1,ss))
	{
		log("create msg_engine_start sniffer_sip_loop failed\n");
		return -1;
	}
    pthread_detach(tid);

	return tid;


}

