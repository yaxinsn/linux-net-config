
#ifndef CONFIG_H
#define CONFIG_H
#include "types_.h"

struct hostip_st
{
    struct in_addr netmask;
    struct in_addr ip;  
    struct in_addr gateway; 
    
};
struct heart_server_st
{
    int interval; /* second */
    char url[256];
    
};
struct callcenter_st
{
    struct in_addr ip;//net 
    unsigned short port;//host
    
};

struct tapeserver_st
{   
    unsigned short mainport;
    unsigned short spareport;
    struct in_addr mainip;
    struct in_addr spareip;
};

struct password_st
{
    unsigned char password[32];
};


struct config_st
{
	struct hostip_st hostip;
	struct heart_server_st heart_ser;
	struct callcenter_st call;
	struct tapeserver_st tape;
	struct password_st pwd;
};
int get_config(struct config_st* c);
int show_config(struct config_st* c);


#endif

