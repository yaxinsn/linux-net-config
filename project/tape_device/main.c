/*****************************************************************

2017/9/24 10:04:09 liudan 
读取整个文件，转成字符串，并完成config的定义。

文件分类：

main.c


*****************************************************************/
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

#include "log.h"
#include "config.h"
#include "upload.h"
//#include "thread_msg_engine.h"

struct config_st g_config;
void main_get_config()
{
	
	memset(&g_config,0,sizeof(g_config));
	get_config(&g_config);
	show_config(&g_config);
}
int main(int argc,char* argv[])
{
	int ret;
	main_get_config();
	
	ret = uploader_start();
	if(ret !=0)
	{
		log("uploader start error, exit\n");
		exit(1);
	}
	return 0;
}