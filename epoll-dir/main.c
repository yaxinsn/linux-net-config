
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


{
	int epfd ,n ,i;
	struct epoll_event event;
	struct epoll_event events[256];
    ssize_t lLen;

	epfd = epoll_create(256);
	event.data.fd = hotplug;
	event.events= EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, hotplug, &event);

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

