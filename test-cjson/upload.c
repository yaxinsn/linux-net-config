/**********************************************

维护好与服务器的联系。
对外提供一个upload函数。
把用户希望的数据上报到服务器。

upload也会是一个线程。
他生成一个skb的队列。

**********************************************/

#include <sys/types.h>
#include <stdlib.h>

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <netdb.h>
#include <arpa/inet.h> 
#include <stdarg.h>
#include "log.h"


typedef struct _sync
{
	pthread_mutex_t lock;
	int volatile    waiting;
	int 			sockfd[2];		//0-wirte,1-read;
}_sync_t;
typedef struct _entry
{
	TAILQ_ENTRY(_entry) node;
	long 				stamp;
	int 				type;
	char				msg[0];
}_entry_st;

typedef TAILQ_HEAD(__list, _entry)  entry_head_t;



typedef struct upload_frame_list
{

	_sync_t		 sync;
	entry_head_t frame_head;
	
}upload_frame_list_t;

upload_frame_list_t  upload_ctx;



int upload_init(void)
{
	
}


int wakeup_upload(void)
{
	wirte(upload_ctx.sync.sockfd[0],"1",1);
	return 0;
}

int upload_send(void* content,int len)
{


	wakeup_upload();

}

upload_pthread()
{
	
}

