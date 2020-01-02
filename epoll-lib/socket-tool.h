#ifndef _SOCKET_TOOL_H
#define _SOCKET_TOOL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>


#include <net/route.h>

#include <fcntl.h>
static __inline__  int setnonblocking( int   sock)
{
     int  opts;
    opts = fcntl(sock,F_GETFL);
     if (opts < 0 )
    {
        perror( " fcntl(sock,GETFL) " );
        return -1;
    }
    opts  =  opts | O_NONBLOCK;
     if (fcntl(sock,F_SETFL,opts) < 0 )
    {
        perror( " fcntl(sock,SETFL,opts) " );
        return -2;
    }
	return 0;
}

static __inline__  int set_sock_recvbuffer( int   sock,int* len)
{
    setsockopt(sock, SOL_SOCKET, SO_RCVBUF, len, sizeof(*len));
    return 0;
}
static __inline__ void set_close_on_exec(int fd)
{
    int flags = fcntl(fd, F_GETFD);
    flags |= FD_CLOEXEC;
    fcntl(fd, F_SETFD, flags);
    return;
}

#endif //_SOCKET_TOOL_H
