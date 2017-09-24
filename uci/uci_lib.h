#ifndef AUCI_LIB_H
#define AUCI_LIB_H


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
#include <arpa/inet.h> 

#define SECT_NAME_LEN 32

typedef int (*write_cfg_cb)(FILE* fp,char** section_name,int section_count,int found_count,void* arg);

typedef int (*read_cfg_cb)(FILE* fp,void* result);

char* skip_space(char* s);
char* kill_end_enterkey(char* s);



int auci_read_section
	(char* file_name,char** sect_name,int section_name_count,
	read_cfg_cb handler,void* section_result);

int auci_write_param
	(char* file_name,char** sect_name,int section_name_count,
	char* param_name,char* param_value);

int auci_write_section 
	(char* file_name,char** sect_name,int section_name_count,
	write_cfg_cb handler,void* new_content);//not debug it .2017-08-1. happy China of army Day

int auci_read_param
	(char* file_name,char** sect_name,int section_name_count,
	char* param_name,char** param_value);

char** auci_parse_param_path(char* line,int* count);
int auci_get_param_value(char* file_name,char* param_path,char** value);

int auci_set_param_value(char* file_name,char* param_path,char* value);
int auci_write_sections_name_before
    (FILE* fp_w,char** section_name,int section_count,int found_sect_count);
int auci_write_sections_name_after
    (FILE* fp_w,char** section_name,int section_count,int found_sect_count);

/**************************convert function********************************/

struct _element_st;


typedef int (*_write_struct_handle)(FILE* fp,void* src,
                           struct _element_st* e,void* arg1,void* arg2);

typedef int (*_read_struct_handle)(void* dest,
                           void *   value,
                           struct _element_st* e,void* arg1,void* arg2);


struct handler
{
    _read_struct_handle read;
    _write_struct_handle write;
};

enum data_type
{
    DTYPE_PRIVATE = 0,
    DTYPE_BOOL,
    DTYPE_INT,
    DTYPE_CHAR_ARRAY,
    DTYPE_STR,    
    DTYPE_MACADDR,
    DTYPE_IPADDR,
    DTYPE_STRUCT,
    DTYPE_STRUCT_ARRAY,
    DTYPE_STRUCT_POINT,
};
typedef struct _element_st{
    /*  */
    char* key;
    enum data_type dtype;
    int offset;
    /* options: */
    int size;
    struct _element_st* sub_element;
    struct handler private_handler;
}_element;


#define OFFSETOF(TYPE,MEMBER) ((int) &((TYPE*)0)->MEMBER)

int common_write_section_cb(FILE* fp, void* arg,_element* parse_ele,int tab_count);
int common_read_section_cb(FILE* fp,void* arg,_element* parse_ele);
int auci_common_write_section 
	(char* file_name,char** sect_name,int section_name_count,
	void* new_content,_element* elments);//not debug it .2017-08-1. happy China of army Day

int auci_common_read_section
	(char* file_name,char** sect_name,int section_name_count,
	void* section_result,_element* elments);

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(X) sizeof(X)/sizeof(X[0])
#endif




#endif  //AUCI_LIB_H
