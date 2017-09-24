/*****************************************************************

2017/9/24 10:04:09 liudan 
config

*****************************************************************/

#include <stdlib.h>
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <stdio.h>
/*

          
           struct in_addr {
               uint32_t       s_addr; 
           };
*/
struct hostip_st
{
    struct in_addr netmask;
    struct in_addr ip;  
    struct in_addr gateway; 
    
};
struct heart_server
{
    int interval; /* second */
    char url[256];
    
};
struct callcenter
{
    struct in_addr ip;//net 
    unsigned short port;//host
    
};

struct tapeserver
{   
    unsigned short mainport;
    unsigned short spareport;
    struct in_addr mainip;
    struct in_addr spareip;
};

struct password
{
    unsigned char passwod[32];
};

#define CONF_ "./base.config"

char buf[65536];

void read_file()
{
	FILE *fp;
 
    char ch;

    if((fp=fopen(CONF_,"r"))==NULL) 
    {
      printf("Cannot open file.\n");
      exit(1);
    }

    while((ch=fgetc(fp)) != EOF) {
     
    }
}

