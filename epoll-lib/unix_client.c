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

#define log(fmt,...)  printf("pid %lu |"fmt,getpid(),##__VA_ARGS__);

#define UNIX_SOCKET_PATH    "/tmp/abcd"

int sock_open_unix(const char *path)
{
	int ret, fd;
	struct sockaddr_un addr;
	struct sockaddr_un my_addr;
    int len;

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd < 0)
	{

		return -1;
    }

    //bind my_addr
	memset(&my_addr, 0, sizeof (struct sockaddr_un));
    my_addr.sun_family = AF_UNIX;
    sprintf(my_addr.sun_path, "/tmp/.agent-shell-log-%d", getpid());
    unlink(my_addr.sun_path);

    len = sizeof (my_addr.sun_family) + strlen (my_addr.sun_path);
    if(bind(fd, (struct sockaddr *) &my_addr, len))
    {
        log("ERROR , bind failed <%s> \n",strerror (errno));
    }

    chmod(my_addr.sun_path,S_IRWXU|S_IRGRP|S_IROTH);

    //connect service.
	memset(&addr, 0, sizeof (struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, path, strlen (path));

	ret = connect(fd, (struct sockaddr *) &addr, sizeof(addr.sun_family) + strlen(addr.sun_path));
	if (ret < 0) {

		close(fd);
		return -1;
	}

	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

    log(" socket  fd %d \n",fd);
	{	char b[124];
		sprintf(b,"I am %u",getpid());
		write(fd,b,124);
	}
	return fd;
}

int main(void)
{
	int ret = sock_open_unix(UNIX_SOCKET_PATH);

	while(1)
	{
		sleep(1);
	}
	return 0;
}
