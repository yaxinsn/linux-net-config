/*
    liudan       2014.03.03         init
    
*/
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
#include <arpa/inet.h> 


#include "uci_lib.h"
#define ARRAY_SIZE(X) sizeof(X)/sizeof(X[0])

#define PRAMA_NAME "auci"

void print_usage(void)
{
	printf("%s -f config_file get param <name>;  param must be absolute path\n",PRAMA_NAME);
	printf("%s -f config_file get section <name> \n",PRAMA_NAME);
	printf("%s -f config_file set param <name> <value>\n",PRAMA_NAME);
	printf("%s -f config_file add section <name>\n",PRAMA_NAME);
	printf("%s -h\n",PRAMA_NAME);
	exit(0);
}
static  int __read_section_handler(FILE* fp,void* arg)
{
	
	char buf[1024] = {0};
	char* p;
	while(fgets(buf,sizeof(buf),fp))
	{
	 	p = strchr(buf,'}');
		if(p)
			break;
		p = strchr(buf,'{');
		if(p)
			break;
		p = skip_space(buf);
		p = kill_end_enterkey(p);
		printf("%s\n",p);
	}
	return 0;
}


int cmd_get_section(char* file_name,char* section)
{
	int ret;
	
	char sect_name[2][32] = {{0},{0}};
	
	sprintf(sect_name[0],section);
	ret = auci_read_section(file_name,&section,1,
						__read_section_handler,NULL);
	return 0;
}


int parse_param_path(char* line, char** sec,char** param)
{
	 char* pv = 0;
	 *sec = skip_space(line);
	 pv = strchr(*sec,'.');
	 
	 if(!pv)
	 	return -1;
	 *param = pv+1;
	 *pv=0;
	 return 0;
}
int cmd_set_param(char* file_name,char* param_path,char* param_value)
{
	int ret = 0;
	
	char* sec = 0;
	char* param_name;


	if(0 != parse_param_path(param_path,&sec,&param_name))
	{
		printf("ERROR param\n");
		exit(0);
	}
	
	ret =  auci_write_param(file_name, &sec,1,
		param_name,param_value);
	if(ret !=0)
		fprintf(stderr,"ERROR: not find the param\n");
	return 0;
}

int cmd_get_param(char* file_name,char* param_path)
{
	int ret = 0;
	char sect_name[2][32] = {{0},{0}};
	char* sec = 0;
	char* param_name;
	char* param_value = 0;

	if(0 != parse_param_path(param_path,&sec,&param_name))
	{
		fprintf(stderr,"ERROR: not find the param\n");
		exit(0);
	}
	
	sprintf(sect_name[0],sec);
	ret =  auci_read_param(file_name, &sec,1,
		param_name,&param_value);
	if(param_value)
	{
		printf("%s\n",param_value);
		free(param_value);
	}
	else
		fprintf(stderr,"ERROR: not find the param\n");
	return 0;
}
struct sub_st
{
    int Suba;
    char Sub_char_array[32];
};
_element sub_st_parse_[] = 
{

    {"Suba",DTYPE_INT,OFFSETOF(struct sub_st,Suba),1},
    {"Sub_char_array",DTYPE_CHAR_ARRAY,OFFSETOF(struct sub_st,Sub_char_array),32},
    {NULL,},
};

struct test_ssid_st
{
    
    bool Enable;
    int Status; 
    char char_array[32];
    char* str;
    unsigned char mac[6];
    struct in_addr ip;
    struct sub_st sub;
};
_element test_st_parse_[] = 
{
    {"Enable",DTYPE_BOOL,OFFSETOF(struct test_ssid_st,Enable)},
    {"Status",DTYPE_INT,OFFSETOF(struct test_ssid_st,Status)},
    {"mac",DTYPE_MACADDR,OFFSETOF(struct test_ssid_st,mac)}, 
    {"sub",DTYPE_STRUCT,OFFSETOF(struct test_ssid_st,sub),0,sub_st_parse_,{NULL,NULL}},
    {"char_array",DTYPE_CHAR_ARRAY,OFFSETOF(struct test_ssid_st,char_array),32}, 
    {"str",DTYPE_STR,OFFSETOF(struct test_ssid_st,str)},     
    {"ip",DTYPE_IPADDR,OFFSETOF(struct test_ssid_st,ip)},   
    {NULL,},      
};
#define _CONFIG_FILE     "/tmp/test.conf"

#if 0
int __write_test_cb(FILE* fp,char** section_name,int section_count,int found_count,void* arg)
{
    char prev_format[20] = {0};

    auci_write_sections_name_before(fp,section_name,section_count,found_count);
	/* write the new param. */
	
	memset(prev_format,'\t',section_count);



	common_write_section_cb(fp,arg,test_st_parse_,section_count);
	

    auci_write_sections_name_after(fp,section_name,section_count,found_count);
}


int __read_test_cb(FILE* fp,void* result)
{
    common_read_section_cb(fp,result,test_st_parse_);

}


int __read_test_object(struct test_ssid_st* cfg,int id)
{
    int ret;
    char id_str[4];
    char* sect_name[] = {
    "Device",
    "WiFi",
    "SSID",
    id_str,
    };
    sprintf(id_str,"%d",id);
    ret = auci_read_section(_CONFIG_FILE,
        sect_name,ARRAY_SIZE(sect_name),__read_test_cb,cfg);
    return ret;
}

int __write_test_object(struct test_ssid_st* cfg,int id)
{    int ret;
    char id_str[4];
    char* sect_name[] = {
    "Device",
    "WiFi",
    "SSID",
    id_str,
    };
    sprintf(id_str,"%d",id);
    
    ret = auci_write_section(_CONFIG_FILE,
        sect_name,ARRAY_SIZE(sect_name),__write_test_cb,cfg);


}
#else
int __read_test_object(struct test_ssid_st* cfg,int id)
{
    int ret;
    char id_str[4];
    char* sect_name[] = {
        "Device",
        "WiFi",
        "SSID",
        id_str,
    };
    sprintf(id_str,"%d",id);
    ret = auci_common_read_section(_CONFIG_FILE,
        sect_name,ARRAY_SIZE(sect_name),cfg,test_st_parse_);
    return ret;
}

int __write_test_object(struct test_ssid_st* cfg,int id)
{    int ret;
    char id_str[4];
    char* sect_name[] = {
        "Device",
        "WiFi",
        "SSID",
        id_str,
    };
    sprintf(id_str,"%d",id);
    
    ret = auci_common_write_section(_CONFIG_FILE,
        sect_name,ARRAY_SIZE(sect_name),cfg,test_st_parse_);


}

#endif
int test()
{
    struct test_ssid_st a;
    struct test_ssid_st b;
    
    a.Enable = true;
    memcpy(a.char_array,"testa",5);
    a.char_array[5] = 0;
    a.Status = 18;
    a.ip.s_addr = 0x01a0b0c0;
    a.mac[0] = 0xcd;
    a.mac[1] = 0xaa;
    a.mac[2] = 0x00;
    a.mac[3] = 0x11;
    a.mac[4] = 0x22;
    a.mac[5] = 0x33;
    a.str = "str1a23b456c789";
    a.sub.Suba = 9;
     memcpy(a.sub.Sub_char_array,"xxxdfdf",8);
     a.char_array[8] = 0;

    __write_test_object(&a,2);
        
    __read_test_object(&b,2);
    __write_test_object(&b,3);
    free(b.str);
}
int main(int argc,char** argv)
{

	//printf("argc %d \n",argc);
	argc--;
	argv++;
	if(5 == argc)
	{
		if(strcmp(argv[0],"-f"))
		{
			goto usage;
		}
		if(!strcmp(argv[2],"get"))
		{
			if(!strcmp(argv[3],"section"))
			{
				cmd_get_section(argv[1],argv[4]);//
			}
			else if(!strcmp(argv[3],"param"))
			{
				cmd_get_param(argv[1],argv[4]);
			}
			else
				goto usage;
		}
	}
	else if(6 == argc)
	{
		if(!strcmp(argv[2],"set"))
		{
			if(!strcmp(argv[3],"param"))
			{
				printf("---------\n");
				cmd_set_param(argv[1],argv[4],argv[5]);
			}
			else
				goto usage;
		}
		else
			goto usage;
		
	}
	else if(1 == argc)
	{
	    if(!strcmp(argv[0],"test"))
	    {
	        test();
	    }
	}
	else
		goto usage;


return 0;

usage:
	print_usage();
	
	return 0 ;
}

