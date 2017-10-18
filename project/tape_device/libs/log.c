

#if 0
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
#endif

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>

#include "log.h"

void _logger_file(const char* file_name, const char* func, int line, const char* fmt,...)
{
    int fd;
    va_list args;
    FILE* log_fp=NULL;
   // static int log_line = 0;
    char buf_time[62];
    int l;
    struct stat st_a;
   
    if (log_fp == NULL) {
        if ((log_fp = fopen(file_name, "a+")) == NULL)
          return;
    }
    time_t a;
    time(&a);

    l = sprintf(buf_time,"%s",ctime(&a));
    buf_time[l-1] = 0;
    fprintf(log_fp,"%s",buf_time);

    fprintf(log_fp," |%s|%d| ",func,line);
    va_start(args,fmt);
    vfprintf(log_fp,fmt,args);
    va_end(args);
   // log_line++;
    fflush(log_fp);
    if(!stat(file_name,&st_a))
    {
    	if(st_a.st_size >= 4096*10000)
    	{
	        fd = fileno(log_fp);
	        ftruncate(fd, 0);
	        lseek(fd, 0, SEEK_SET);
    	}
    }
#if 0    
    if(log_line >= 400)
    {
        fd = fileno(log_fp);
        ftruncate(fd, 0);
        lseek(fd, 0, SEEK_SET);
        log_line = 0;
    }
#endif    
    fclose(log_fp);
    return;
}

