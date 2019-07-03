
#include <sys/inotify.h>
#include <sys/epoll.h>
 #include <stdio.h>
 
  #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

	   
struct epoll_event stEv;
struct inotify_event stInEv;
	
#define EPOLL_MAX_FD 255
typedef void (*_EPOLL_CALLBACK)(void *);

typedef struct _epoll_args
{
	int fd;
	void *pArg;
	_EPOLL_CALLBACK pfEpollCallback;
}_EPOLL_ARGS_S;

typedef struct _epoll_Ino
{
	char* fileName;
	_EPOLL_CALLBACK pfEpollCallback;
}_epoll_Ino_s;

/*********************************************************************/


listen_usb()
{
int fd = open("dirname", O_RDONLY);	
}
read_testfile(void *pIn)
{
	
	char buf[1024] = {0};
	_EPOLL_ARGS_S *pstEpollUserData = pIn;
	 read(pstEpollUserData->fd,buf,1024);
	 printf("%s:%d <%s> \n",__func__,__LINE__,buf);
}

void listen_testfile()
{
	 int fd = open("./testfile",O_CREAT|O_RDONLY);
	 if(fd <0)
	 {
		 perror("listen_testfile");
		 exit(1);
	 }
	 resigterEpoll(fd,read_testfile);
	 
	 printf("%s:%d end \n",__func__,__LINE__);
}





/***************************************************************************/




int iEpollFd;
int iInotifyFd

int resigterEpollInofity(char* fileName,_EPOLL_CALLBACK func)
{
	  int fd = inotify_add_watch(iInotifyFd, fileName, IN_MODIFY);
    if (-1 == fd)
    {
		return -1;
	}
	_epoll_Ino_s* p = (_epoll_Ino_s *)malloc(sizeof(_epoll_Ino_s));
	if(NULL == p)
	{
		printf("resigterEpoll: ");
	}
}
int resigterEpoll(int IFd,_EPOLL_CALLBACK func)
{
	struct epoll_event FdEvent              = {0};
	_EPOLL_ARGS_S* pstEpollArg = (_EPOLL_ARGS_S *)malloc(sizeof(_EPOLL_ARGS_S));
	if(NULL == pstEpollArg)
	{
		printf("::Malloc pstEpollArg Fail!\n");
		//exit(1);
	}
	bzero(pstEpollArg,sizeof(_EPOLL_ARGS_S));

	pstEpollArg->fd = IFd;
	pstEpollArg->pfEpollCallback = func;
	FdEvent.data.ptr = pstEpollArg;
	FdEvent.events = (EPOLLIN|EPOLLERR);

	epoll_ctl(iEpollFd, EPOLL_CTL_ADD, IFd,&FdEvent);
}
static void* createEpollPthread(void *argv)
{
	int iFdNum                                     = 0;
	int i                                          = 0;
	struct epoll_event astEvents[EPOLL_MAX_FD]     = {0};
	_EPOLL_ARGS_S *pstEpollArg              = NULL;

	
	int iInotifyFd = inotify_init();
	  
	iEpollFd = epoll_create(EPOLL_MAX_FD);
	if(iEpollFd < 0)
	{
		perror("main::epoll_create Fail!");
		exit(1);
	}

	for(;;)
	{
		iFdNum = epoll_wait(iEpollFd,astEvents,EPOLL_MAX_FD,-1);
		for(i = 0;i < iFdNum;i++)
		{
			if(astEvents[i].events & EPOLLIN)
			{
				pstEpollArg = astEvents[i].data.ptr;
				pstEpollArg->pfEpollCallback(astEvents[i].data.ptr);
			}
			else if(astEvents[i].events & EPOLLERR)
			{

			}
		}
	}
}



int main(int argc,char *argv[])
{
	int tid;
	if(pthread_create(&tid,NULL,createEpollPthread,(void*)0))
	{
		perror("pthread_create!");
	}
	listen_testfile();
	//pthread_wait();
	while(1)
	{
		sleep(100);
	}
	return 0;
}



