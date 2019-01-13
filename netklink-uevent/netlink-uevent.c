#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/user.h>
#include <sys/un.h>
#include <linux/types.h>
#include <linux/netlink.h>


#include <sys/epoll.h>

#define HOTPLUG_BUFFER_SIZE     1024
#define HOTPLUG_NUM_ENVP        32
#define OBJECT_SIZE         512

#define  _ErrorLog printf
#define _DebugLog printf


struct uevent {
    void *next;
    char buffer[HOTPLUG_BUFFER_SIZE + OBJECT_SIZE];
    char *devpath;
    char *action;
    char *envp[HOTPLUG_NUM_ENVP];
};

static struct uevent * alloc_uevent (void)
{
    return (struct uevent *)malloc(sizeof(struct uevent));
}


int __set_noblock(int sfd)
{
	int flags, ret = 0;

	flags= fcntl(sfd, F_GETFL,0);
	if(flags < 0){
		perror("fcntl");
		ret = -1;
		goto err;
	}

	flags|= O_NONBLOCK;
	ret = fcntl(sfd, F_SETFL, flags);
	if(ret < 0){
		perror("fcntl");
		goto err;
	}
err:
	return ret;
}

int __hotplug_init(void)
{
	const int buffersize = 1024;
	int ret = -1;
	struct sockaddr_nl snl;

	bzero(&snl, sizeof(struct sockaddr_nl));
	snl.nl_family = AF_NETLINK;
	snl.nl_pid = getpid();
	snl.nl_groups = 1;

	int s = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if(s < 0){
		perror("socket");
		goto err;
	}

	ret = __set_noblock(s);

	if(ret < 0){
		goto err;
	}

	setsockopt(s, SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize));

	ret = bind(s, (struct sockaddr *)&snl, sizeof(struct sockaddr_nl));
	if (ret < 0){
		goto err;
	}

	return s;
err:
	if(s > 0){
		close(s);
	}
	return ret;
}

static int _UkeyRemove(char *buf, size_t ulLen, ssize_t lRecvLen)
{
    int iIsRemove = 0;
    int i;
    char *pcPos;
    char *pcKey;
    char *pcBuffer;
    size_t  ulBufPos;
    ssize_t lBuflen;
    size_t  ulKeylen;
    lBuflen = lRecvLen;
    struct uevent stUevent;
    struct uevent *pstUevent;

    if (lBuflen < 0)
    {
        if (errno != EINTR)
        {
            _ErrorLog("error receiving message!");
        }
        goto end;
    }

    /* skip header */
    ulBufPos = strlen(buf) + 1;
    if (ulBufPos < sizeof("a@/d") || ulBufPos >= ulLen) {
        _ErrorLog("invalid message length\n");
        goto end;
    }

    /* check message header */
    if (strstr(buf, "@/") == NULL) {
        _ErrorLog("unrecognized message header");
        goto end;
    }
		

    pstUevent = &stUevent;

    if ((size_t)lBuflen > ulLen-1)
    {
        lBuflen = ulLen-1;
    }

    /*
     * Copy the shared receive buffer contents to buffer private
     * to this uevent so we can immediately reuse the shared buffer.
     */
    memcpy(pstUevent->buffer, buf, HOTPLUG_BUFFER_SIZE + OBJECT_SIZE);
    pcBuffer = pstUevent->buffer;
    pcBuffer[lBuflen] = '\0';

    /* save start of payload */
    ulBufPos = strlen(pcBuffer) + 1;

    /* action string */
    pstUevent->action = pcBuffer;
    pcPos = strchr(pcBuffer, '@');
    if (!pcPos) {
        _ErrorLog("bad action string '%s'", pcBuffer);
        goto end;
    }
    pcPos[0] = '\0';

    /* sysfs path */
    pstUevent->devpath = &pcPos[1];

    /* hotplug events have the environment attached - reconstruct envp[] */
    for (i = 0; (ulBufPos < (size_t)lBuflen) && (i < HOTPLUG_NUM_ENVP-1); i++)
    {
        pcKey = &pcBuffer[ulBufPos];
        ulKeylen = strlen(pcKey);
        pstUevent->envp[i] = pcKey;
        ulBufPos += ulKeylen + 1;
    }
    pstUevent->envp[i] = NULL;
    _DebugLog("uevent '%s' from '%s'.\n", pstUevent->action, pstUevent->devpath);

    /* print payload environment */
    for (i = 0; pstUevent->envp[i] != NULL; i++)
    {
        _DebugLog("%s\n", pstUevent->envp[i]);


    }

    if ((i >= 7)
        && (0 == strncmp(pstUevent->envp[0], "ACTION=remove", 13))
        && (0 == strncmp(pstUevent->envp[2], "SUBSYSTEM=usb", 13))
        && (0 == strncmp(pstUevent->envp[6], "DEVTYPE=usb_device", 18)))
    {
        iIsRemove = 1;
    }

end:

    return iIsRemove;
}
void show_recv_buf(char* buf,int len)
{
	int  i = 0;
	while(i<len)
	{
		printf("%s\n",buf+i);
		i += strlen(buf+i)+1;
	}
	//printf("%s:%d <%s>\n",__func__,__LINE__,buf);
	printf(" ---------------------------------\n");
}

int __monitor_cycle(int fd, char *userid)
{
	int epfd ,n ,i;
	struct epoll_event event;
	struct epoll_event events[256];
    int len;


	epfd = epoll_create(256);
	event.data.fd = fd;
	event.events= EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);




    while (1)
    {
        n = epoll_wait(epfd, events, 256, -1);

        for(i = 0; i < n; i++)
        {
            if(events[i].data.fd == fd)
            {
                char buf[HOTPLUG_BUFFER_SIZE + OBJECT_SIZE] = {0};

                len = recv(fd, buf, sizeof(buf), 0);
				printf("len %d \n",len);
				show_recv_buf(buf,len);
#if 0
                if (_UkeyRemove(buf, sizeof(buf), len))
                {
                    printf("------------------------one ukey has remove---------------------------------\r\n");
                }
#endif
                event.data.fd = fd;
                event.events= EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &event);

            }
        }
    }

 }


int main()
{
	char *userid = "0";
	int hotplug_sock;

	hotplug_sock = __hotplug_init();

	__monitor_cycle(hotplug_sock, userid);


 }

