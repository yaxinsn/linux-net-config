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
#include <assert.h>

#include "log.h"
#include "types_.h"

#include "upload.h"
#include "thread_msg_engine.h"
#include "config.h"
#include "linux-utils.h"
#include "wake_utils.h"


#define TYPE_REQUEST 1
#define TYPE_RESPONSE 2
#define TYPE_FINISH  3
#define TYPE_UPLOAD  4


struct msg_head
{
	u8 type;
	u8 resv[3];
};



typedef struct upload_ctx_st
{
	struct msg_engine_ctx msg_eng;
	int main_fd; /*main server fd */
	int bak_fd; /* back server fd */
	int warn_event;// 1  main server发送失败。 2 back server 发送失败。
}upload_ctx_t;



upload_ctx_t  upload_ctx;
#define PERROR log

static int connect_tcp_socket(struct in_addr * server,int serverPort)
{
    int ret = 0;
    int error = 0;
    int sk = -1;
    struct sockaddr_in sin; 
    socklen_t sl;
    fd_set  rds; 
    fd_set  wrs;
//    fd_set  tmp_rds; 
    struct timeval exp = {0};

    sk = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    log("socket = %d", sk);
    assert(sk>0);

    setnonblocking(sk); 
    
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET; 
    sin.sin_port   = htons(serverPort); 
    sin.sin_addr.s_addr = server->s_addr;
	
	log("Connecting... port=%d, ip=%s", ntohs(sin.sin_port), inet_ntoa(sin.sin_addr));
	
	ret = connect(sk, (struct sockaddr*)&sin, sizeof(sin));
    if( 0 != ret ){
        if( EINPROGRESS != errno ){
            log("connect %d errno %d/%s", ret, errno, strerror(errno));
            log("Prog exit....%d",ret);
            return ret;
        }

        FD_ZERO(&rds);
        FD_ZERO(&wrs);
        FD_SET(sk, &rds);
        FD_SET(sk, &wrs);        
        exp.tv_sec = 75;/* 等待 connect过程 x s */
		

        ret = select(sk+1, &rds, &wrs, NULL, &exp);
        if( ret <= 0 ){
            PERROR("nonblocking SYN+ACK timeout");
            PERROR("Prog exit....%d", ret);
            return ret;
        }

        error = 0 ; 
        sl = sizeof(error); 

        ret = getsockopt(sk, SOL_SOCKET, SO_ERROR, (void*)&error, &sl);
        if( 0>ret ){
            log("nonblocking state check failed.");
            log("Prog exit....%d", ret);
			close(sk);
            return ret;
        }

        if( 0 != error ){
            log("nonblocking connect failed, error = %d/%s", error, strerror(error));
            log("Prog exit....");
			close(sk);
            return error;
        }      
    }
	return sk;
}

int __send_to_server(void* msg,int len)
{
	int ret;
	if(upload_ctx.main_fd > 0)
	{
		ret = send(upload_ctx.main_fd,msg,len,0);
		if(ret != 0)
		{
			upload_ctx.warn_event = 1;
		}
	}
	else
	{
		
	}
	if(upload_ctx.bak_fd > 0)
	{
		ret = send(upload_ctx.bak_fd,msg,len,0);
		if(ret != 0)
			upload_ctx.warn_event = 2;
	}
	return 0;
	
}
static int __send_talking(void* msg,u32 len)
{
	struct phone_msg* pm = (struct phone_msg*)msg;
	struct talking_mesg* tm = NULL;
	struct msg_head* msg_head = NULL;
	
	if(len <= sizeof(struct phone_msg)){
		log("pm data is empty, can't send\n");
		return -1;
	}
	tm = (struct talking_mesg*)pm->data;
	msg_head = (struct msg_head*)&tm->resv;
	msg_head->type = TYPE_UPLOAD;
	__send_to_server(tm,len-sizeof(struct phone_msg));
	return 0;
}

static void upload_close_socket()
{
	if(upload_ctx.main_fd >0)
		close(upload_ctx.main_fd);
	upload_ctx.main_fd = 0;
	
	if(upload_ctx.bak_fd >0)
		close(upload_ctx.bak_fd);	
	upload_ctx.bak_fd = 0;	
}

static void upload_open_socket()
{
	extern struct config_st g_config;
	upload_close_socket();
  	upload_ctx.main_fd = connect_tcp_socket(&g_config.tape.mainip,g_config.tape.mainport);
  	upload_ctx.bak_fd = connect_tcp_socket(&g_config.tape.spareip,g_config.tape.spareport);

}

static int _ring_down_send_finish()
{
	struct msg_head msg;
	int len = sizeof(struct msg_head);
	
	msg.type = TYPE_FINISH;
	return __send_to_server(&msg,len);
	return 0;
}
static int _ring_down_()
{
	_ring_down_send_finish();
	upload_close_socket();
	return 0;
}

static int _ring_up_send_request()
{
	struct msg_head msg;
	int len = sizeof(struct msg_head);
	
	msg.type = TYPE_REQUEST;
	__send_to_server(&msg,len);
	return 0;
}
static int _ring_up_()
{
	
	upload_open_socket();
	//send a request 
	_ring_up_send_request();
	return 0;
}

static int __upload_msg_handle(void* msg,int len,struct msg_engine_ctx* me)
{
	struct phone_msg* pm = msg;
	switch(pm->event)
	{
		case RING_UP:
			log("RING UP \n\n");
			_ring_up_();
			
			break;
			
		case TALKING:
			__send_talking(msg,len);
			break;
		case RING_DOWN:
			log("RING DOWN \n\n");
			_ring_down_();
			break;
		default:
			log("unknown event %d \n",pm->event);
	}
	return 0;
}


int upload_init(void)
{
	//read config;
	upload_open_socket();
	upload_ctx.msg_eng.cb_func = __upload_msg_handle;
	return 0;
}


int uploader_start(void)
{
	pthread_t tid;
	int ttid = 0;
	upload_init();
	tid = msg_engine_start(&upload_ctx.msg_eng,"upload");
	ttid = (int)tid;
	if(ttid == -1)
		return -1;
	
	return tid;

}

int uploader_push_msg(void* msg,int len)
{

	return msg_engine_push_entry(&upload_ctx.msg_eng,msg,len);
	
	return 0; 
}


