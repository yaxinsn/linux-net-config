
#include <sys/inotify.h>
#include <sys/epoll.h>
 #include <stdio.h>
 
  #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
#include <unistd.h>

	    #include <sys/types.h>
       #include <dirent.h>

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
	int iWatchFd;
	char* fileName;
	_EPOLL_CALLBACK Callback;
}_epoll_Ino_s;

/*********************************************************************/
#if 0
read_usb(void *pIn)
{
	char buf[1024] = {0};
	_epoll_Ino_s *pstInoData = pIn;
	 
	        DIR *pdir = opendir(pstInoData->fileName);

	 if(pdir 0)////////dead
	 {
		 perror(" read_usb ");
		 printf("read_usb file %s \n", pstInoData->fileName);
		 exit(0);
	 }
	 
	 read(fd,buf,1024);
	 printf("%s:%d <%s> \n",__func__,__LINE__,buf);
}
void listen_usb()
{
	 resigterEpollInofity("/sys/bus/usb/devices",read_usb);
}
#endif

void read_testfile(void *pIn)
{
	
	char buf[1024] = {0};
	_epoll_Ino_s *pstInoData = pIn;
	 int fd = open(pstInoData->fileName,O_RDONLY);
	 if(fd <0)
	 {
		 perror(" read_testfile ");
		 printf("read_testfile file %s \n", pstInoData->fileName);
		 exit(0);
	 }
	 read(fd,buf,1024);
	 printf("%s:%d <%s> \n",__func__,__LINE__,buf);
}

void listen_testfile()
{
	
	
	 resigterEpollInofity("/proc/kmsg",read_testfile);
	 
	 printf("%s:%d end \n",__func__,__LINE__);
}





/***************************************************************************/




int iEpollFd;
int iInotifyFd;
_epoll_Ino_s * epoll_inotify_array[128];
int epoll_inotify_array_count = 0;



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
		printf("resigterEpollInofity: ");
	}
	p->iWatchFd = fd;
	p->fileName = strdup(fileName);
	p->Callback = func;
	if(epoll_inotify_array_count<128)
	{
		epoll_inotify_array[epoll_inotify_array_count] = p;
		epoll_inotify_array_count++;
	}
	return 0;
}
void readInotify(void *pIn)
{
	
	char buf[1024] = {0};
	_EPOLL_ARGS_S *pstEpollUserData = pIn;
	
    struct inotify_event stInEv;
	int wfd = 0;
	int iRet;
	int i;
	iRet = read(pstEpollUserData->fd, &stInEv, sizeof(stInEv));
    if (iRet != sizeof(stInEv))
    {
            printf("The data length readed %d is too small.\n", iRet);

		return;
    }

        
	 for(i = 0;i<epoll_inotify_array_count;i++)
	 {
		 if (stInEv.wd == epoll_inotify_array[i]->iWatchFd)
		 {
			 printf("%s:%d %d\n",__func__,__LINE__,i);
			 
			 epoll_inotify_array[i]->Callback(epoll_inotify_array[i]);
			 epoll_inotify_array[i]->iWatchFd = inotify_add_watch(iInotifyFd, epoll_inotify_array[i]->fileName, IN_MODIFY);
			 
			 return ;
		 }
	 }
	 printf("%s:%d \n",__func__,__LINE__);
}
void listenInotify()
{

	
	 resigterEpoll(iInotifyFd,readInotify);
	 printf("%s:%d \n",__func__,__LINE__);
}

void read_kmsg(void *pIn)
{
	
	char buf[1024] = {0};
	_EPOLL_ARGS_S *pstEpollUserData = pIn;
	int iret = read(pstEpollUserData->fd,buf,1024);
	printf("ksmg %s \n",buf);
}	

void listen_kmsg()
{
	int fd = open("/proc/kmsg",O_RDONLY);

	printf("-%s:%d- fd %d \n",__func__,__LINE__,fd);
	 resigterEpoll(fd,read_kmsg);
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
	FdEvent.events = (EPOLLIN|EPOLLERR|EPOLLET);
	printf("-%s:%d- fd %d \n",__func__,__LINE__,IFd);
	epoll_ctl(iEpollFd, EPOLL_CTL_ADD, IFd,&FdEvent);
}
static void* createEpollPthread(void *argv)
{
	int iFdNum                                     = 0;
	int i                                          = 0;
	struct epoll_event astEvents[EPOLL_MAX_FD]     = {0};
	_EPOLL_ARGS_S *pstEpollArg              = NULL;

	
	  
	iEpollFd = epoll_create(EPOLL_MAX_FD);
	listenInotify();
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
	iInotifyFd = inotify_init();
	listen_testfile();
	listen_kmsg();
	while(1)
	{
		sleep(100);
	}
	return 0;
}



