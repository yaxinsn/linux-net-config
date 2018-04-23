
#ifndef __LOG_H
#define __LOG_H
#include "types_.h"
#include <string.h>

#include <sys/errno.h>

void _logger_file(const char* file_name, const char* func, int line, const char* fmt,...);

#define log(fmt,...)  \
    _logger_file("/tmp/hzivy-main.log",__func__,__LINE__,fmt,##__VA_ARGS__);  
    
#define log_err(fmt,...)  \
    _logger_file("/tmp/hzivy-main.log",__func__,__LINE__,"ERROR| "fmt,##__VA_ARGS__); 


#define log_errno(fmt,...)  \
    _logger_file("/tmp/hzivy-main.log",__func__,__LINE__,"ERROR| errnoinfo <%s> | "fmt,strerror(errno),##__VA_ARGS__); 


	
#define sip_log(fmt,...)  \
    _logger_file("/tmp/hzivy-sip.log",__func__,__LINE__,fmt,##__VA_ARGS__);  


#define sip_log_err(fmt,...)  \
						sip_log("ERROR|"fmt,##__VA_ARGS__); 

#define FREE(x)  do { if(x != NULL) free(x); x=NULL;} while(0);     

#endif //__LOG_H