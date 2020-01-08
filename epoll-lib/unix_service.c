


#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>


#include <stdint.h>
#include <string.h>

#include <sys/errno.h>
#include <pthread.h>


#include <unistd.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <sys/un.h>
#include "socket-tool.h"
#define log_error(fmt,...)  printf("[%s:%d ] pid %lu |"fmt,__func__,__LINE__,getpid(),##__VA_ARGS__);

#define UNIX_SOCKET_PATH    "/tmp/abcd"


#include "epoll_lib.h"


typedef struct _ctx_t
{
    epoll_ctx_st    *epoll_st;
    int             server_socket;
}
ctx_t;



ctx_t g_ctx;
static void _read (void* arg);
#define MAX_CONNECT_COUNT    20
static int  _serv_un (const char *path)
{
  int ret;
  int sock, len;
  struct sockaddr_un serv;
  mode_t old_mask;

  /* First of all, unlink existing socket */
  unlink (path);

  /* Set umask */
  old_mask = umask (0011);

  /* Make UNIX domain socket. */
  sock = socket (AF_UNIX, SOCK_STREAM, 0);
  if (sock < 0)
    {
      log_error("Can't create zserv unix socket: %s",
                 strerror (errno));
      log_error ("zebra can't provide full functionality due to above error");
      return -1;
    }


  /* Make server socket. */
  memset (&serv, 0, sizeof (struct sockaddr_un));
  serv.sun_family = AF_UNIX;
  strncpy (serv.sun_path, path, strlen (path));
#ifdef HAVE_STRUCT_SOCKADDR_UN_SUN_LEN
  len = serv.sun_len = SUN_LEN(&serv);
#else
  len = sizeof (serv.sun_family) + strlen (serv.sun_path);
#endif /* HAVE_STRUCT_SOCKADDR_UN_SUN_LEN */

  ret = bind (sock, (struct sockaddr *) &serv, len);
  if (ret < 0)
    {
      log_error ("Can't bind to unix socket %s: %s",
                 path, strerror (errno));
      log_error ("zebra can't provide full functionality due to above error");
      close (sock);
      return -1;
    }

  ret = listen (sock, MAX_CONNECT_COUNT);
  if (ret < 0)
    {
      log_error ("Can't listen to unix socket %s: %s",
                 path, strerror (errno));
      log_error ("zebra can't provide full functionality due to above error");
      close (sock);
      return -1;
    }

  umask (old_mask);
    return sock;
}
static void _accept (void* arg)
{

    int client_sock;
    struct sockaddr_in client;
    socklen_t len;

    epoll_args_st* ep_arg = ( epoll_args_st*)arg;
    int accept_sock = ep_arg->fd;



    len = sizeof (struct sockaddr_in);
    client_sock = accept (accept_sock, (struct sockaddr *) &client, &len);

    if (client_sock < 0)
    {
      log_error ("Can't accept zebra socket: %s", strerror (errno));
      return;
    }

  /* Make client socket non-blocking.  */
    setnonblocking(client_sock);
    resigter_epoll(g_ctx.epoll_st,client_sock,_read);
/*
    client's pid. when it is dead, unresig epoll . // todo.
*/

}
#define MAX_READ_SIZE  10240
static void _read (void* arg)
{
    char buffer[MAX_READ_SIZE] = {0};
    ssize_t ret;

    epoll_args_st* ep_arg = (epoll_args_st*)arg;
    int fd = ep_arg->fd;

    ret = read (fd, buffer, MAX_READ_SIZE);
    if(ret > 0)
    {
        log_error(" ret %d \n",ret);
    }


}

int startup_server_socket(void)
{
    int sock;
    sock = _serv_un(UNIX_SOCKET_PATH);
    if(sock == -1)
    {
        return -1;
    }
    g_ctx.server_socket = sock;
    resigter_epoll(g_ctx.epoll_st, sock, _accept);
    return 0;
}



int main(void)
{
	
    g_ctx.epoll_st = create_epoll(-1);
    if ( g_ctx.epoll_st == NULL)
    {
        return -1;
    }
	startup_server_socket();
	
	while(1)
	{
		
		epoll_loop(g_ctx.epoll_st);
	}
}