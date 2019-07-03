 #include <stdio.h>
#include <pthread.h>

       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
 #include <sys/epoll.h>

 #include <sys/inotify.h>

#include <unistd.h>
#include <stdbool.h>

int inotify_fd;
_listen_read(char* filename )
{
	
    struct inotify_event stInEv;
	int iRet;
	inotify_fd = inotify_init();
	int watch_fd = inotify_add_watch(inotify_fd, filename, IN_MODIFY);
	while(1)
	{
		iRet = read(inotify_fd, &stInEv, sizeof(stInEv));
		if(iRet >=0)
		{
			 if(stInEv.wd == watch_fd)
			 {
				 printf("bingo! \n");
			 }
		}
	}
}


static void* listen_config_file(void* arg)
{
	char* filename = (char*)arg;
    pthread_detach(pthread_self());
	_listen_read(filename);
}

bool monitor_file_modifed(char* filename)
{
  pthread_t tid;

  if(pthread_create(&tid,NULL,listen_config_file,filename))
  {
      printf("pthread creat listen_config_file Failed  \n");
      return false;
  }
  return true;

}


int main(int argc,char* argv[])
{
	monitor_file_modifed("./xxx");
	while(1)
	{
		sleep(10);
	}
	return 0;
}