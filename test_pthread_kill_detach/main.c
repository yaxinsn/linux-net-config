

#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <arpa/inet.h>
#include <pthread.h>

 #include <signal.h>

void sighandler(int s)
{
	int retval = 3;
	printf("sighandler %d  \n",pthread_self());
	pthread_exit(&retval);
}

void* sniffer_rtp_loop(void* arg)
{
	
	signal(SIGQUIT, sighandler);
	while(1)
	{
		printf("I am rtp loop\n");
		sleep(1);
	}
}

pthread_t __sniffer_rtp_start(void)
{
	pthread_t tid;
	
	if(pthread_create(&tid,NULL,sniffer_rtp_loop,NULL))
	{
		printf("create pthread_create sniffer_rtp_loop failed\n");
		return -1;
	}

	return tid;

}
int thread_kill(pthread_t thread_id)
{
	int kill_ret = pthread_kill(thread_id,SIGQUIT);  
	printf("%s:%d ret %d \n",__func__,__LINE__,kill_ret);
	if(kill_ret == ESRCH)  
		printf("the tid is not exist\n");  
	else if(kill_ret == EINVAL)  
		printf("unvalide signal\n");  
	else  
	printf("the tid is exist\n"); 
	
	return 0;
}
void* sniffer_sip_loop(void* arg)
{
	int i = 0;
	pthread_t tid;
	while(1)
	{
		printf("I am rtp sip i %d\n",i);
		sleep(2);
		i++;
		if( i == 3){
			tid = __sniffer_rtp_start();
			pthread_detach(tid);
		}
		if(i ==6){
			printf("I will kill the tid %d \n",tid);
			thread_kill(tid);
		}
	}
}

pthread_t __sniffer_sip_start(void)
{
	pthread_t tid;
	
	if(pthread_create(&tid,NULL,sniffer_sip_loop,NULL))
	{
		printf("create pthread_create sniffer_sip_loop failed\n");
		return -1;
	}

	return tid;

}

int main()
{
	pthread_t tid;
	tid = __sniffer_sip_start();
	pthread_join(tid,NULL);
	printf("end \n");
	
}
