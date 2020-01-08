/************************************************************
  Copyright (C), 1988-2999, WINICSSEC Tech. Co., Ltd.
  FileName:
  Author:        Version :          Date:
  Description:   epoll的库
                    建立epoll句柄。

  Version:
  Function List:

  History:
      <author>  <time>   <version >   <desc>
      dan.liu   2019/12、3     1.0     build this module
***********************************************************/

#include <sys/syscall.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <sys/errno.h>

#include "epoll_lib.h"

#include "socket-tool.h"





int resigter_epoll(epoll_ctx_st* ctx, int fd,_EPOLL_CALLBACK func)
{
	struct epoll_event FdEvent              = {0};
	epoll_args_st* pst_epoll = (epoll_args_st *)malloc(sizeof(epoll_args_st));
	if(NULL == pst_epoll)
	{
		epoll_debug("::Malloc pstEpollArg Fail!\n");
		return -1;
	}
	bzero(pst_epoll,sizeof(*pst_epoll));

	pst_epoll->fd = fd;
	pst_epoll->epoll_callback = func;
	FdEvent.data.ptr = pst_epoll;
	FdEvent.events = (EPOLLIN | EPOLLERR | EPOLLHUP);
    if(ctx->epoll_fd == 0)
    {
        perror("g_epoll_ctx.epoll_fd == 0");
        return -1;
    }
	epoll_ctl( ctx->epoll_fd, EPOLL_CTL_ADD, fd,&FdEvent); //以后加上错误判断。
    list_add(&pst_epoll->node ,&ctx->head);

    return 0;
}

int unresigter_epoll(epoll_ctx_st* ctx,int fd)
{
    epoll_args_st* p;
    epoll_args_st* n;
    int iret = -1;

    pthread_mutex_lock(&ctx->head_lock);
	list_for_each_entry_safe(p, n, &ctx->head, node)
	{
	    if(p->fd == fd)
	    {
            iret = epoll_ctl(ctx->epoll_fd, EPOLL_CTL_DEL, fd, NULL);//以后加上错误判断。

            list_del(&p->node);
            free(p);
            iret = 0;
            break;
        }
	}
    pthread_mutex_unlock(&ctx->head_lock);
    return iret;
}

void close_one_fd(epoll_ctx_st* ctx,int fd)
{
    unresigter_epoll(ctx,fd);
    close(fd);

}

void epoll_loop(epoll_ctx_st* ctx)
{
	int ifd_num                                    = 0;
	int i                                          = 0;
	struct epoll_event astEvents[EPOLL_MAX_FD]     ;
	epoll_args_st *pstEpollArg                      = NULL;

    memset(&astEvents,0,sizeof(astEvents));


	for(;;)
	{
		ifd_num = epoll_wait(ctx->epoll_fd,astEvents,EPOLL_MAX_FD,ctx->timeout);
        if(ctx->stop)
        {
            epoll_debug("stop!\n");
            break;
        }
#if 0
        /*epoll事件错误.*/
        if (ifd_num < 0)
        {
            epoll_debug("epoll event err! <%s> ",strerror (errno));
            break;
        }
#endif
        /*无事件返回.*/
        else if (ifd_num == 0)
        {
            continue;
        }
		for(i = 0;i < ifd_num;i++)
		{

			if ((astEvents[i].events & EPOLLIN)
			    && (!((astEvents[i].events) & EPOLLERR))
			    && (!((astEvents[i].events) & EPOLLHUP)))
			{
                pstEpollArg = astEvents[i].data.ptr;
                pstEpollArg->epoll_callback(astEvents[i].data.ptr);
			}
			else if((astEvents[i].events) & EPOLLERR)
			{

				pstEpollArg = astEvents[i].data.ptr;
                close_one_fd(ctx,pstEpollArg->fd);
			}
			else
			{
				pstEpollArg = astEvents[i].data.ptr;

			    epoll_debug("i %d events %x pstEpollArg->fd %d \n",i,(astEvents[i].events),pstEpollArg->fd);
			    close_one_fd(ctx,pstEpollArg->fd);
			}
		}
	}
}


epoll_ctx_st* create_epoll(int timeout)

{
    int fd = 0;
    epoll_ctx_st* ctx;
    ctx = (epoll_ctx_st*)malloc(sizeof(epoll_ctx_st));
    if(!ctx)
    {
        return NULL;
    }

    INIT_LIST_HEAD(&ctx->head);
    pthread_mutex_init(&ctx->head_lock, NULL);
	//epoll_create(size) size参数无用。
	fd = epoll_create(1);
	if(fd < 0)
	{
        epoll_debug ("main::epoll_create Fail!  %s", strerror (errno));
        free(ctx);
		return NULL;
	}
    set_close_on_exec(fd);
    ctx->epoll_fd = fd;
    ctx->stop = 0;
    ctx->timeout = timeout;
    return ctx;
}

void epoll_stop(epoll_ctx_st* ctx)
{
    if(ctx)
    {
        ctx->stop = 1;
    }
}
void close_epoll(epoll_ctx_st* ctx)
{
    if(ctx)
    {
        close(ctx->epoll_fd);
        free(ctx);
    }
}

