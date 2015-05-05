
#include <stdio.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "setopt.h"


int ns_setopt(int cmd,void* data,int len)
{
	int sk;
	int ret;
	sk = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(sk)
	{
		ret = setsockopt(sk,0,cmd,data,len);
		close(sk);
	}
	else
	{
		return -9;
	}
	return ret;
}
int ns_getopt(int cmd,void* data,int len)
{
	int sk;
	int ret;
	sk = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(sk)
	{
		ret = getsockopt(sk,0,cmd,data,&len);
		close(sk);
	}
	else
	{
		return -10;
	}
	return ret;
}


int main(int argc,char* argv[])
{

	kinfo_t a;
	int ret;
	a.count = 5;
	a.countl = 0x6;
	ret = ns_setopt(NS_SO_SET_COUNT,&a,sizeof(a));
	printf("ret ---- %d \n",ret);
	return 0;
}

