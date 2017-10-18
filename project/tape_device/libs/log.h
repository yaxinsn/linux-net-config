
#ifndef __LOG_H
#define __LOG_H
#include "types_.h"
void _logger_file(const char* file_name, const char* func, int line, const char* fmt,...);

#define log(fmt,...)  \
    _logger_file("/tmp/hzivy-main.log",__func__,__LINE__,fmt"\n",##__VA_ARGS__);  
    
#define FREE(X)  do { if(x != NULL) free(x); x=NULL;} while(0);     

#endif //__LOG_H