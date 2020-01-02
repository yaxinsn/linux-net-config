
#ifndef _EPOLL_LIB_H
#define _EPOLL_LIB_H
#include <pthread.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "list.h"
//#include "log.h"
/*
dependence : pthread, list

*/
#ifdef __cplusplus
        extern "C" {
#endif


#define EPOLL_MAX_FD 1024

typedef void (*_EPOLL_CALLBACK)(void *);

typedef struct _epoll_args
{
    struct list_head    node;

	int 				fd;
	void 				*p_arg;
	_EPOLL_CALLBACK 	epoll_callback;
}epoll_args_st;

#define EPOLL_NEVER_TIMEOUT -1

typedef struct epoll_ctx_t
{

    int                 epoll_fd;
    pthread_mutex_t     head_lock;
    struct list_head    head;
    int                 timeout;//
    int                 stop;
}epoll_ctx_st;

//与整个工程的系统日志进行对接。
#define epoll_debug(fmt, arg...) printf("%s-%d " fmt,  __FUNCTION__, __LINE__, ##arg)
//#define epoll_debug(fmt, arg...) log_debug(fmt,   ##arg)

/************************************************************
Function ： create_epoll
Desc:
             建立epoll ctx
             count 表示侦听多少个，最大为1024
             timeout为超时的时间，单位是s, -1表示永不超时。

return:     返回 0成功， -1 失败。
***********************************************************/
epoll_ctx_st* create_epoll(int count,int timeout);


/************************************************************
Function ： epoll_loop
Desc:
            使用死循环，侦听epoll,回调各个处理函数，一般需要一个独立的线程来启动loop
return:     返回 0成功， -1 失败。
***********************************************************/
void epoll_loop(epoll_ctx_st* ctx);

/************************************************************
Function ： epoll_stop
Desc:
            从epoll_loop中跳出来，
return:     返回 0成功， -1 失败。
***********************************************************/
void epoll_stop(epoll_ctx_st* ctx);
/************************************************************
Function ： close_epoll
Desc:
            关闭epoll.
return:
***********************************************************/
void close_epoll(epoll_ctx_st* ctx);



int resigter_epoll(epoll_ctx_st* ctx, int fd,_EPOLL_CALLBACK func);

int unresigter_epoll(epoll_ctx_st* ctx,int fd);



#ifdef __cplusplus
	}
#endif

#endif //_EPOLL_LIB_H


