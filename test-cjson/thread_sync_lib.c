


int  setnonblocking( int  sock)
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

int sync_init(int sv[2])
{
	int ret;
	ret = socketpair(AF_UNIX,SOCK_STREAM, 0, sv);
	if(ret !=0)
	{
		log("xxx");
		return -EFAULT;
	}

	fcntl(sv[0],F_SETFDFD_CLOEXEC);
	fcntl(sv[1],F_SETFDFD_CLOEXEC);
	setnonblocking(sv[0]);
	setnonblocking(sv[1]);
	return 0;
}



