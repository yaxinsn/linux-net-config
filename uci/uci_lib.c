/*
    user config interface
    
    liudan       2017.07.31         init
    
*/
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

#include "uci_lib.h"


 struct read_param_function_st
 {
	 char** param_value;
	 char* param_name;
 };
 
 struct write_param_function_st
 {
	 char* param_value;
	 char* param_name;
 };


static void 
_logger_file(const char* file_name, const char* func, int line, const char* fmt,...)
{
    int fd;
    va_list args;
    static FILE* log_fp;
    static int log_line = 0;
    char buf_time[62];
    int l;
   
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
    log_line++;
    fflush(log_fp);
    if(log_line >= 400)
    {
        fd = fileno(log_fp);
        ftruncate(fd, 0);
        lseek(fd, 0, SEEK_SET);
        log_line = 0;
    }
    return;
}
#define cw_logger(fmt,...) \
    _logger_file("/tmp/auci-lib.log",__func__,__LINE__,fmt"\n",##__VA_ARGS__);

char* skip_space(char* s);

int __check_mac_addr(char* mac_str)
{
	int hexnum=0;

	if(strlen(mac_str) != 17){
		cw_logger("Error mac len:%d\n", strlen(mac_str));
		return 0;
	}

	char *strtmp = strtok(mac_str, ":");

	while(strtmp != NULL){
		if(strlen(strtmp) != 2)
			return 0;

		if((strtmp[0]>='0' && strtmp[0]<='9')
			|| (strtmp[0]>='A' && strtmp[0]<='F')
			|| (strtmp[0]>='a' && strtmp[0]<='f')){		
			if((strtmp[1]>='0' && strtmp[1]<='9')
				|| (strtmp[1]>='A' && strtmp[1]<='F')
				|| (strtmp[1]>='a' && strtmp[1]<='f')){
				hexnum++;
				strtmp=strtok(NULL, ":");
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}

	if(hexnum != 6)
		return 0;

	return 1;
}

int __check_ipaddr(const char* str)
{
    struct in_addr ipaddr;
    int ret;

    ret = inet_pton(AF_INET, str, (void *)&ipaddr);
    if(ret != 1)
        return 0;

    return 1;
}


//////////////////////////////////////////////////
/******************base function of operation config file *********************/

static int __locate_section_for_write(char** section_name,int section_name_count,FILE* f_read,FILE* f_write)
{
	int j = 0;

	char endkey[30] = {0};
    char line[1024];
	char* p = NULL;
	FILE* fp_r = f_read;
	FILE* fp_w = f_write;
	
	sprintf(endkey, "}%s\n", section_name[0]);
 	for(; j<section_name_count; j++)
	{
    	while(fgets(line,sizeof(line),fp_r)&&
			((p = skip_space(line)) != NULL) )
    	{
      	  	if(!strncmp(p,section_name[j],strlen(section_name[j]))
		  		&&(p[strlen(section_name[j])] == '{'))
     	   	{     	   
				
  		      	if(j == section_name_count-1){
    	        	return j+1; // locate success  return value is  section_name_count
   	     		}
				sprintf(endkey, "}%s\n", section_name[j]);
         	    fputs(line,fp_w);
         	    
           		break;
           	}
			else
			{
				/* encounter the endkey. */
				if(!strcmp(p, endkey))
				{
					fseek(fp_r, -(strlen(line)), SEEK_CUR);
					return j; // new
				}
				fputs(line,fp_w);
			}	
		}
	}
    fflush(fp_w);
	sync();
	
	return 0;
}

static int __locate_section_for_read(
char** section_name,int section_name_count,FILE* f_read)
{
	int i = 0;
	

	char* p = NULL;

    char line[1024];
	char *pend = NULL;
	FILE* fp_r = f_read;
	
	 for(i=0; i<section_name_count; i++){
		while(fgets(line,sizeof(line),fp_r)&&
			((p = skip_space(line)) != NULL))
		{
			if(!strncmp(p,section_name[i],strlen(section_name[i]))
				&&(p[strlen(section_name[i])] == '{'))
			{
					if(i == section_name_count-1){
						 return 1;
					}
					break;
			}
			else
			{
				if((pend = strchr(p, '}'))
					&& i>0 && !strcmp(section_name[i-1], pend+1))
				{
						return 0;//at multi class,the file's point beyond the "}father"
				}
			}
		}	
	}
	 return 0;//at file end,not found.
}


static  int __check_cfg_file_link(const char* cfg_file)
{
    struct stat st;
//    char str[100];
	int ret;
	
    struct stat st_a;


	char bak_a_name[100];
	
	
	memset(&st,0,sizeof(struct stat));
    if(lstat(cfg_file, &st))//ap.conf not exist
    {
        cw_logger("ERROR:file <%s> is not exist; lstat <%s>",cfg_file,strerror(errno));
        // ap.conf not exist, find ap.conf-a
        sprintf(bak_a_name,"%s-a",cfg_file);
        if(!lstat(bak_a_name,&st_a))//bak_a exist
        {
        
		cw_logger("INFO: bak-a exist, symlink bak_a to <%s>",cfg_file);
            ret = symlink(bak_a_name,cfg_file);
            if(ret)
            {
                cw_logger(
                "ERROR bak-a  symlink failed  <%s>",strerror(errno));
                       
            }
            return 0;
        }
        //ap.conf ap.conf-a not exist, find ap.conf-b.
          sprintf(bak_a_name,"%s-b",cfg_file);
            if(!lstat(bak_a_name,&st_a))//bak_b exist
        {
				cw_logger("INFO: bak-a exist, symlink bak_b to <%s>",cfg_file);
            ret = symlink(bak_a_name,cfg_file);
            if(ret)            
            {
                cw_logger("ERROR bak-b  symlink failed  <%s>",strerror(errno));         
            }
            return 0;
        }
    }
    
	if(!S_ISLNK(st.st_mode))//not link file
    {
        cw_logger("INFO: <%s> is not a link file",cfg_file);  
        sprintf(bak_a_name,"%s-a",cfg_file);
        ret = rename(cfg_file,bak_a_name);
        if(ret)
            cw_logger("ERROR: rename <%s> <%s>",cfg_file,strerror(errno));
            
        ret = symlink(bak_a_name,cfg_file);
        if(ret)
        {
            cw_logger(
                "ERROR: symlink <%s> <%s>",cfg_file,strerror(errno));           
        }
    }
    
    return 0;
}
static char*  __select_write_cfg_file(const char* cfg_file,char* real_file)
{
    char buf[100] 	= {0};
    char name1[100] = {0};
    char name2[100] = {0};


	__check_cfg_file_link(cfg_file);

    sprintf(name1,"%s-a",cfg_file);
    sprintf(name2,"%s-b",cfg_file);
	
    if(readlink(cfg_file,buf,sizeof(buf)) > 0)
    {
        if(!strncmp(name1,buf,strlen(buf)))
            strncpy(real_file,name2,strlen(name2));
        else if(!strncmp(name2,buf,strlen(buf)))
            strncpy(real_file,name1,strlen(name1));
        else
        {
            __check_cfg_file_link(cfg_file);
            strncpy(real_file,name2,strlen(name2));
        }
    }
    return real_file;
}
pid_t lock_test(int fd,int type,off_t offset,int whence,off_t len)
{
    struct flock lock;
    
    lock.l_type     = type;
    lock.l_start    = offset;
    lock.l_whence   = whence;
    lock.l_len      = len;
    if (fcntl(fd,F_GETLK,&lock) < 0)
         return -1; //   error;

    if(lock.l_type == F_UNLCK)
        return 0;

    return lock.l_pid;
         
}
#define is_read_lockable(fd,offset,whence,len)  \
    (lock_test((fd),F_RDLCK,(offset),(whence),(len)) ==0)
#define is_write_lockable(fd,offset,whence,len)  \
    (lock_test((fd),F_WRLCK,(offset),(whence),(len)) ==0)    
int lock_reg(int fd,int cmd,int type,off_t offset,int whence,off_t len)
{
    struct flock lock;
    
    lock.l_type     = type;
    lock.l_start    = offset;
    lock.l_whence   = whence;
    lock.l_len      = len;
    return fcntl(fd,cmd,&lock);
}
#define read_lock(fd,offset,whence,len)  \
    (lock_reg((fd),F_SETLK,F_RDLCK,(offset),(whence),(len)))
    
#define write_lock(fd,offset,whence,len)  \
        (lock_reg((fd),F_SETLK,F_WRLCK,(offset),(whence),(len)))

#define un_lock(fd,offset,whence,len)  \
        (lock_reg((fd),F_SETLK,F_UNLCK,(offset),(whence),(len)))        

//int w_lock_fd = 0;

static int __try_conf_lock_(char* file_name)
{
    int i=0;
	int w_lock_fd;
	char lock_name[100];
	
	sprintf(lock_name,"%s.lock",file_name);

    w_lock_fd = open(lock_name,O_WRONLY|O_CREAT|FD_CLOEXEC);
    if(w_lock_fd <=0)
        return -1;
        
    while(!is_write_lockable(w_lock_fd,0,SEEK_SET,0))//fail
    {
        i++;
        if(i > 10)
        {
            return -1;
        }
        sleep(1);
        
    }
	write_lock(w_lock_fd,0,SEEK_SET,0);        
    return w_lock_fd;

}
int __lock_conf_lock_(int w_lock_fd,char* file_name)
{
	char lock_name[100];
	sprintf(lock_name,"%s.lock",file_name);
    if(w_lock_fd <= 0)
       w_lock_fd = open(lock_name,O_RDWR|FD_CLOEXEC);
    return write_lock(w_lock_fd,0,SEEK_SET,0);
}

static int __unlock_conf_lock_(int w_lock_fd,char* file_name)
{
    
	char lock_name[100];
	sprintf(lock_name,"%s.lock",file_name);
    if(w_lock_fd <= 0)
       w_lock_fd = open(lock_name,O_RDWR|FD_CLOEXEC);
    
    return  un_lock(w_lock_fd,0,SEEK_SET,0);
}


int auci_read_section 
	(char* file_name,char** section_name,int section_name_count,
	read_cfg_cb handler,void* section_result)
{
   
    FILE* fp;
	int lock_fd;
    int locate = 0;
    int ret = 9;
    
	
    if((lock_fd = __try_conf_lock_(file_name)) == -1)
    {
	    cw_logger("err __try_conf_lock_ error");
	    return -1;
    }
    __check_cfg_file_link(file_name);



    fp = fopen(file_name,"r");
    if(!fp){

	
	
        return -1;
    }
    
    locate = __locate_section_for_read(section_name, section_name_count, fp);
	
	

    //enter handler
    if(locate)
        ret = handler(fp,section_result);
    else
    {
        //error 
        ret = -2;
    }
    //unlock file
    fclose(fp);
	__unlock_conf_lock_(lock_fd,file_name);
    return ret;
}
static int __write_section_cfg_end_context2
	(FILE* fp_r,FILE* fp_w,char** section_name,int section_name_count, int locate)
{
	int i;
	i = 0;
	char line[1024];
	/* wirte the end of context at read_file	*/

	while(fgets(line,sizeof(line),fp_r))
	{
		fputs(line,fp_w);
	}
	
    
    fflush(fp_w);
	sync();

	return 0;
}

static int __write_section_cfg_end_context
	(FILE* fp_r,FILE* fp_w,char** section_name,int section_name_count, int locate)
{
	int i;
	i = 0;
	char* p = NULL;
	char line[1024];
	/* wirte the end of context at read_file	*/

	if(locate != section_name_count)
	{
		while(fgets(line,sizeof(line),fp_r))
		{
			fputs(line,fp_w);
		}
	}else{
		/* skip the old section at the read_file */
		/* locate the section_name from read_file */
		locate = 0;
		while(fgets(line,sizeof(line),fp_r)&&
			((p = skip_space(line)) != NULL))
		{
			if(strchr(p,'{'))
			{
				i++;
			}
			if(strchr(p, '}'))
			{
				if(i == 0)
				{
					locate = 1;
					break;
				}
				i--;
			}
		}
		if(locate == 1)
		{
			while(fgets(line,sizeof(line),fp_r))
			{
				fputs(line,fp_w);
			}
		}
	}
    
    fflush(fp_w);
	sync();

	return 0;
}

int auci_write_section 
	(char* file_name,char** section_name,int section_name_count,
	write_cfg_cb handler,void* new_content)
{  
    char* c_file = file_name;
    char w_file[128] = {0};
    FILE* fp_r;
    FILE* fp_w;
	int lock_fd;
    int locate = 0;
    int ret = -9;

	if((lock_fd = __try_conf_lock_(file_name)) == -1)
    {
	    cw_logger("err __try_conf_lock_ error");
	    return -1;
    }

    __select_write_cfg_file(file_name,w_file);
    

    fp_r = fopen(c_file,"r");
    if(!fp_r){
        
        cw_logger("err open(r) %s",c_file);
		__unlock_conf_lock_(lock_fd,file_name);
        return -1;
    }

    fp_w = fopen(w_file,"w");
    if(!fp_w)
    {
        cw_logger("err open(w) %s",w_file);
        fclose(fp_r);	
		__unlock_conf_lock_(lock_fd,file_name);
        return -2;
    }
    

	locate = __locate_section_for_write(section_name,section_name_count,fp_r,fp_w);
    //enter handler
    ret = handler(fp_w,section_name,section_name_count,locate,new_content);
	
	__write_section_cfg_end_context(fp_r,fp_w,section_name,section_name_count,locate);

    /* write over! */
    fflush(fp_w);
    sync();

    fclose(fp_w);
    fclose(fp_r);
    
    ret = unlink(c_file);
    if(ret)
        cw_logger( 
	        "unlink %d ",ret);
    
    ret = symlink(w_file,c_file);
    if(ret)
    {
    	cw_logger(
	        "configfile is <%s> updated file <%s>," 
	        "and link it. symlink %d <%s> section name <%s>",
	        c_file,w_file,
	        ret,strerror(errno),section_name[0]);    
          
    }
	__unlock_conf_lock_(lock_fd,file_name);
    return ret;
}

int auci_write_sections_name_before(FILE* fp_w,char** section_name,int section_count,int found_sect_count)
{
    int i,k;
    

	/* wirte the section header */
	if(found_sect_count < section_count)
	{
		for(i = found_sect_count; i< section_count;i++)
		{
			for(k = 0;k<i;k++)
				fprintf(fp_w,"\t");
			
			fprintf(fp_w,"%s{\n",section_name[i]);
		}
	}
	else if(found_sect_count == section_count)
	{
	    
			for(k = 0;k<section_count-1;k++)
				fprintf(fp_w,"\t");
			
			fprintf(fp_w,"%s{\n",section_name[section_count-1]);
	}
	return 0;
}
int auci_write_sections_name_after(FILE* fp_w,char** section_name,int section_count,int found_sect_count)
{
    int i,k;
    
	/* wirte the section end */
    if(found_sect_count < section_count)
    {
        for(i = section_count-1; i>= found_sect_count; i--)
        {
            for(k = 0;k<i;k++)
                fprintf(fp_w,"\t");
            fprintf(fp_w,"}%s\n",section_name[i]);
        }
    }
    else if(found_sect_count == section_count)
    {
        
            for(k = 0;k<section_count-1;k++)
                fprintf(fp_w,"\t");
            fprintf(fp_w,"}%s\n",section_name[section_count-1]);
    }
    return 0;
}

//not find the section from read_file, the new the section 
int __auci_write_param_handler2
	(FILE* fp_w,char** section_name,int section_count,int found_sect_count,
	char* param_name,char* param_value)
{
	int i;
	int k;
	
	char prev_format[10]= {0};
	//printf("%s:%d found_sect_count %d\n",__func__,__LINE__,found_sect_count);
	/* wirte the section header */
	if(found_sect_count < section_count)
	{
		for(i = found_sect_count; i< section_count-1;i++)
		{
			for(k = 0;k<i;k++)
				fprintf(fp_w,"\t");
			
			fprintf(fp_w,"%s{\n",section_name[i]);
		}
	}
	
	memset(prev_format,'\t',section_count-1);
	fprintf(fp_w,"%s%s{\n",prev_format,section_name[section_count-1]);
	/* write the new param. */
	memset(prev_format,'\t',section_count);
	fprintf(fp_w,"%s%s=%s\n",prev_format,param_name,param_value);
	
	/* wirte the section end */
	/* write the buttom level section. */
	memset(prev_format,0,10);
//	memset(prev_format,'\t',section_count-1);
//	fprintf(fp_w,"%s}%s\n",prev_format,section_name[section_count-1]);
	/* write the upper level section */
	if(found_sect_count < section_count)
	{
		for(i = section_count-1; i>= found_sect_count; i--)
		{
			for(k = 0;k<i;k++)
				fprintf(fp_w,"\t");
			fprintf(fp_w,"}%s\n",section_name[i]);
		}
	}
    fflush(fp_w);
    sync();
	return 0;
}
//find the section for read_file, so continue to find the param_name.
int __auci_write_param_handler1
	(FILE* fp_w,FILE* fp_r,char** section_name,int section_name_count,char* param_name,char* param_value)
{

    char line[1024];
	char* p = NULL;
	int flag = 0;
	char endkey[33] = {0};
	char prev_format[10]= {0};

	//write the section_name to write_file	
	memset(prev_format,'\t',section_name_count-1);
	fprintf(fp_w,"%s%s{\n",prev_format,section_name[section_name_count-1]);//write the section name.

	//read fp_r, and find the param.
	
	sprintf(endkey, "}%s", section_name[section_name_count-1]);
	while (fgets(line,sizeof(line),fp_r)&&
			((p = skip_space(line)) != NULL))
	{
  	  	if(!strncmp(p,param_name,strlen(param_name))) //find it.
 	   	{
			memset(prev_format,'\t',section_name_count);
			fprintf(fp_w,"%s%s=%s\n",prev_format,param_name,param_value);
       		flag = 1; //not break, must write the other param of the section
       	}
		else
		{
			/* encounter the endkey. so we not find the param */
			if(!strncmp(p, endkey,strlen(endkey)))
			{
				if(flag == 1)//
				{
					//fputs(line,fp_w);//this line is }xxx
					goto end;
					return 0;
					break;//return 
					
				}
				else //write the new one.
				{
					memset(prev_format,'\t',section_name_count);
					fprintf(fp_w,"%s%s=%s\n",prev_format,param_name,param_value);
					//fputs(line,fp_w); //this line is }xxx
					goto end;
				    return 0;
					break;//return 
					
				}
			}
			else
				fputs(line,fp_w);
		}	
	}
	return -1;

end: //write the }xxxx. and bybye
    memset(prev_format,0,sizeof(prev_format));
	memset(prev_format,'\t',section_name_count-1);
	fprintf(fp_w,"%s}%s\n",prev_format,section_name[section_name_count-1]);//write the section name.
	fflush(fp_w);
    sync();
    return 0;
        
}


int auci_write_param
	(char* file_name,char** section_name,int section_name_count,
	char* param_name,char* param_value)
{  
	char* c_file = file_name;
	char w_file[128] = {0};
	FILE* fp_r;
	FILE* fp_w;
	int lock_fd;
	int locate = 0;
	int ret = -9;

	if((lock_fd = __try_conf_lock_(file_name)) == -1)
	{
		cw_logger(
		"err __try_conf_lock_ error");
		return -1;
	}

	__select_write_cfg_file(c_file,w_file);

	fp_r = fopen(c_file,"r");
	if(!fp_r){
		
		cw_logger(
			"err open(r) %s",c_file);
		__unlock_conf_lock_(lock_fd,file_name);
		return -1;
	}

	fp_w = fopen(w_file,"w");
	if(!fp_w)
	{
		cw_logger(
			"err open(w) %s",w_file);
		fclose(fp_r);	
		__unlock_conf_lock_(lock_fd,file_name);
		return -2;
	}
 
	locate = __locate_section_for_write(section_name,section_name_count,fp_r,fp_w);
	//enter handler
	if(locate  == section_name_count)
	{
		
		cw_logger("%s:%d  locate %d == section_name_count,so find the section. \n",__func__,__LINE__,locate);
		__auci_write_param_handler1
			(fp_w,fp_r,section_name,
			section_name_count,param_name,param_value);
	}
	else if(locate < section_name_count) //not find the section . so new it.
	{
	
		cw_logger("%s:%d  locate %d < section_name_count,so not find the section. \n",__func__,__LINE__,locate);
		__auci_write_param_handler2(fp_w,  section_name,  section_name_count, locate,
									param_name,param_value);
        
	}
    
	__write_section_cfg_end_context2(fp_r,fp_w,section_name,section_name_count,locate);
	/* write over! */
	fflush(fp_w);
	sync();

	fclose(fp_w);
	fclose(fp_r);
	
	ret = unlink(c_file);
	if(ret)
		cw_logger( 
			"unlink %d ",ret);
	
	ret = symlink(w_file,c_file);
	if(ret)
	{
		cw_logger(
			"configfile is <%s> updated file <%s>," 
			"and link it. symlink %d <%s> section name <%s>",
			c_file,w_file,
			ret,strerror(errno),section_name[0]);	 
		  
	}
	__unlock_conf_lock_(lock_fd,file_name);
	return ret;
}


static int __parse_line(char* line,char**key,char** v)
{
	
	char *pKey = NULL;
	char *pV = NULL;
	char seps[] = "=";
	static char nullBuf[] = "";
	pKey = strtok(line, seps);
	if(pKey)
	{
    	pV = strtok(NULL, seps);
    	if(pV == NULL)
    	{
    		pV = nullBuf;
    	}
	}
	else
	{	
		return -1;
	}

	pKey = skip_space(pKey);
	pV = skip_space(pV);
	*v = pV;
	*key = pKey;
	return 0;
}
static int __read_param_handler_2(FILE* fp,char* param_name,char** param_value)
{
	char *pKey = NULL;
	char *pV = NULL;
	char* p = NULL;
	char line[1024];
	char* line_end = 0;
	FILE* fp_r = fp;
	
 	while( fgets(line,sizeof(line),fp_r)
		  && ((p = skip_space(line)) != NULL))
	{
		 line_end = strchr(line,'\n');
		 if(line_end){
			 *line_end = 0;
		 }
		if(-1 == __parse_line(p,&pKey,&pV))
			continue;
		
 		if(!strncmp(pKey,param_name,strlen(param_name))) //bingo
		{
			*param_value = strdup(pV);//free it ,when finish to use.
			
			cw_logger("read param <%s>=%s \n",param_name,*param_value);
			return 0;
		}
	}
	
	cw_logger("not find param <%s>\n",param_name);
	return -1;
}

static int __read_param_cb(FILE* fp,void* arg)
{
   struct read_param_function_st* t = (struct read_param_function_st*)arg;
   
	char** param_value = t->param_value;
	char* param_name = t->param_name;

	return __read_param_handler_2(fp,param_name,param_value);

}
int auci_read_param
	(char* file_name,char** section_name,int section_name_count,
	char* param_name,char** param_value)
{
	int ret;
	struct read_param_function_st t;
	t.param_name = param_name;
	t.param_value = param_value;
	
	ret = auci_read_section(file_name,section_name,section_name_count,
			__read_param_cb,&t);
	return ret;
}



/******************base function of operation config file end*********************/

char* skip_space(char* s)
{
    while(s)
    {
        if(s[0]== ' ' || s[0] == '\t' )
            s++;
        else
            break;
    }
    return s;
}

char* kill_end_enterkey(char* s)
{
	if(s[strlen(s)-1] == '\n')
		s[strlen(s)-1] = 0;
	return s;
}
char** parse_param_path(char* line,int* count)
{
    char* array[50] = {0};
    char** path = NULL;
    int i = 0;
    
    char *p = NULL;
    char* in = line;
    
    char seps[] = ".";
    do{
        p = strtok(in, seps);
        if(p)
        {
            array[i] = p;
            i++;
            if(i >= 50)
                break;
        }
        
        in = NULL;    
    }while(p);

    path = (char**)malloc(sizeof(char*)*(i+1));
    if(!path)
        return NULL;
        
    memset(path,0,sizeof(char*)*(i+1));
    memcpy(path,array,sizeof(char*)*i);
    *count = i;
    
    //__log("<%s:%d> param count %d \n",__func__,__LINE__,i);
    return path;


}



int auci_get_param_value(char* file_name,char* param_path,char** value)
{
	int ret = -1;
	int count;
	char** sect_name;
    char* param_path2 = strdup(param_path);
    if(param_path2)
    {
        sect_name = parse_param_path(param_path2,&count);
    	if(sect_name == NULL)
    	    goto end1;
    	ret =  auci_read_param(file_name, sect_name,count-1,
    		sect_name[count-1],value);
    }
	free(sect_name);
end1:
    free(param_path2);
	return ret;
}

int auci_set_param_value(char* file_name, char* param_path, char* value)
{
	int ret = -1;
	int count;
	char** sect_name;
    char* param_path2 = strdup(param_path);
    if(param_path2)
    {
    	sect_name = parse_param_path(param_path2,&count);
    	if(sect_name == NULL)
    	    goto end1;
    	ret =  auci_write_param(file_name, sect_name,count-1,
    		sect_name[count-1],value);
    }
	free(sect_name);
end1:
    free(param_path2);
	return ret;
}
/****************************/

int _common_write_section
 (FILE* fp,char** section_name,int section_count,int found_count,void* arg,_element* parse_ele);

int _common_read_section(FILE* fp,void* arg,_element* parse_ele);

int element_parse_mac(void* dest,
                           void *   value,
                           struct _element_st* e,void* arg1,void* arg2)
{    
    char* v = value;
    int offset = e->offset;
    int intmac[6];
    int i;
    unsigned char* pmac = (unsigned char*)dest+offset;
    v = skip_space(v);
    //printf("peer_parse_mac :: v :%s\n",v);
    if(v)
    sscanf(v,"%02X:%02X:%02X:%02X:%02X:%02X",
             &intmac[0],&intmac[1],&intmac[2],
             &intmac[3],&intmac[4],&intmac[5]);
    for ( i = 0; i < 6; i++ )
    {
        pmac[i] = (unsigned char)intmac[i];
    }
    return 0;
}
int element_output_mac(FILE* fp,void* src,
                             struct _element_st* e,void* arg1,void* arg2)
{
    unsigned char* v = (unsigned char*)((char*)src+e->offset);
    
    fprintf(fp,"%s=%02X:%02X:%02X:%02X:%02X:%02X\n",e->key,
            v[0],v[1],v[2],v[3],v[4],v[5]);
    fflush(fp);
    return 0;
}
int element_parse_ipaddr(void* dest,
                           void *   value,
                           struct _element_st* e,void* arg1,void* arg2)
{
    char* v = value;
    int offset = e->offset;
//    int ret = 0;
    struct in_addr sin_addr;
    struct in_addr* dest_ip = (struct in_addr*)((char*)dest+offset);
    v = skip_space(v);
    if(v)
    {
        inet_pton(AF_INET, v, &sin_addr);        
        dest_ip->s_addr = sin_addr.s_addr;
    }
    
    return 0;
}

int element_output_ipaddr(FILE* fp,void* src,
                            struct _element_st* e,void* arg1,void* arg2)
{
    char ipaddr_str[18] = {0};
    const char* p;
    struct in_addr* src_ip = (struct in_addr*)((char*)src+e->offset);
    struct in_addr d;
    d.s_addr = src_ip->s_addr;

    p =  inet_ntop(AF_INET,&d,ipaddr_str, 18);
    if(p)
    {    
        fprintf(fp,"%s=%s\n",e->key,p);
    }
    else
    {
        fprintf(fp,"%s=NA\n",e->key);
    }
    return 0;
}

int element_parse_int(void* dest,
                           void *   value,
                          struct _element_st* e,void* arg1,void* arg2)
{
    char* v = value;
    int offset = e->offset;
    int ret = 0;
    
    v = skip_space(v);
    if(v)
        ret = atoi(v);
    *(int*)((char*)dest+offset) = ret;
    return 0;
}
 int element_output_int(FILE* fp,void* src,
                             struct _element_st* e,void* arg1,void* arg2)
{
    int v1 = *(int*)((char*)src+e->offset);


    fprintf(fp,"%s=%d\n",e->key,v1);
    
    fflush(fp);
    return 0;
}

 int element_parse_bool(void* dest,
                           void *   value,
                           struct _element_st* e,void* arg1,void* arg2)
{
    char* v = value;
    int offset = e->offset;
    int ret = 0;    
    v = skip_space(v);
    if(v)
        ret = atoi(v);
    *(bool*)((char*)dest+offset) = ret?true:false;
    return 0;
}
int element_output_bool(FILE* fp,void* src,
                            struct _element_st* e,void* arg1,void* arg2)
{
    bool v1 = *(bool*)((char*)src+e->offset);

    fprintf(fp,"%s=%d\n",e->key,v1?1:0);
    fflush(fp);
    return 0;
} 

int element_parse_long(void* dest,
                           void *   value,
                           struct _element_st* e,void* arg1,void* arg2)
{

    char* v = value;
    int offset = e->offset;
    long long ret = 0;
    
    v = skip_space(v);
    if(v)
        ret = atoi(v);
    *(long long*)((char*)dest+offset) = ret;

    return 0;
}

int element_parse_char_array(void* dest,
                           void *   value,
                           struct _element_st* e,void* arg1,void* arg2)
{
    char* v = value;
    int offset = e->offset;
    char* dest_ = ((char*)dest+offset);
   
    v = skip_space(v);
    //printf("element_parse_char_array %s-----\n",v);
    if(v)
        strncpy(dest_,v,e->size);
    return 0;
}                           
int element_output_char_array(FILE* fp,void* src,
                            struct _element_st* e,void* arg1,void* arg2)
{
    
    char* v = (char*)((char*)src+e->offset);
   // printf("element_output_char_array %s =<%s>\n",e->key,v);
    fprintf(fp,"%s=%s\n",e->key,v);
    return 0;
}                            
int element_parse_str(void* dest,
                           void *   value,
                           struct _element_st* e,void* arg1,void* arg2)
{
    char* v = value;
    int offset = e->offset;
    char** dest_ = (char**)((char*)dest+offset);
    v = skip_space(v);
    if(v)
    {
        *dest_ = strdup(v);
    }
    return 0;
}                           
int element_output_str(FILE* fp,void* src,
                            struct _element_st* e,void* arg1,void* arg2)
{
    
    char** v = (char**)((char*)src+e->offset);
    if(*v)
        fprintf(fp,"%s=%s\n",e->key,*v);
    return 0;
}                            
int element_parse_struct(void* dest,
                           void *   value,
                           struct _element_st* e,void* arg1,void* arg2)
{

    FILE* fp = arg1;
    
    int offset = e->offset;
    void* dest_ = (void*)((char*)dest+offset);
    _common_read_section(fp,dest_,e->sub_element);

    return 0;
}                           
int element_output_struct(FILE* fp,void* src,
                            struct _element_st* e,void* arg1,void* arg2)
{
    int offset = e->offset;
    int tabs = (int)arg2+1;
    int k;
    
    void* src_ = (void*)((char*)src+offset);

    //printf("%s:%d--\n",__func__,__LINE__);


	fprintf(fp,"%s{\n",e->key);
    
    printf("%s:%d--src_ %p \n",__func__,__LINE__,src_);
    common_write_section_cb(fp,src_,e->sub_element,tabs);

    //printf("%s:%d--\n",__func__,__LINE__);

	for(k = 0;k<tabs-1;k++)
		fprintf(fp,"\t");
	fprintf(fp,"}%s\n",e->key);

    //printf("%s:%d--\n",__func__,__LINE__);

    return 0;
}   

struct handler _handler [] = 
{
    {NULL,NULL},
    {element_parse_bool,element_output_bool},
    {element_parse_int,element_output_int},
    {element_parse_char_array,element_output_char_array},
    {element_parse_str,element_output_str},
    {element_parse_mac,element_output_mac}, 
    {element_parse_ipaddr,element_output_ipaddr}, 
    {element_parse_struct,element_output_struct},     
};

int _fill_section_cb(char* line,void* arg,_element* parse_ele,FILE* fp)
{
    char* p;
    char* line_end;
    char* pKey;
    char* pV;
    int i = 0;
    p = skip_space(line);
    line_end = strchr(line,'\n');
    if(line_end){
     *line_end = 0;
    }
	if(-1 == __parse_line(p,&pKey,&pV))
        return -1;

//TODO: hash before here 

    while(parse_ele[i].key != NULL)
    {
    	if(!strcmp(pKey,parse_ele[i].key)) //bingo
    	{
            goto handle_it;
    	}
        //bingo struct;
        if(pKey[strlen(pKey)-1] == '{' 
            && (!strncmp(pKey,parse_ele[i].key,strlen(parse_ele[i].key))))
        {
            goto handle_it;
        }
    	i++;
    }
    return 0;
    
handle_it:
    if(parse_ele[i].dtype == DTYPE_PRIVATE)
    {
        if(parse_ele[i].private_handler.read)
            return parse_ele[i].private_handler.read(arg,pV,&parse_ele[i],fp,NULL);
    }
    else
        return _handler[parse_ele[i].dtype].read(arg,pV,&parse_ele[i],fp,NULL);

    return 0;
}
int common_read_section_cb(FILE* fp,void* arg,_element* parse_ele)
{
	  FILE *file = fp;
	  char buf[1024] = {0};
	  char *pEnd = NULL;
	  char* section_end;
 
	 
	  if(!arg)
	  {
		  return -1;
	  }
 
 
	  if(file)
	  {
		  memset(buf,0,sizeof(buf));
		  while(fgets(buf,sizeof(buf),file))
		  {
			 pEnd = strchr(buf,'\n');
			 if(pEnd)
			 {
				 *pEnd = 0;
			 }
			 section_end = strchr(buf,'}');
			 if(section_end)
			 {
				 break;
			 }
			  _fill_section_cb(buf,arg,parse_ele,fp);
 
			  memset(buf,0,sizeof(buf));
		  }
	  }

	  return 0;
 }

int common_write_section_cb
 (FILE* fp, void* arg,_element* parse_ele,int tab_count)
{
    int i=0;
    char tabs[20] = {0};

    if(!arg || !fp)
    {
        return -1;
    }
    memset(tabs,'\t',tab_count);
    while(parse_ele[i].key != NULL)
    {
        if(parse_ele[i].dtype == DTYPE_PRIVATE)
        {
            if(parse_ele[i].private_handler.write)
            {
                fprintf(fp,"%s",tabs);
                parse_ele[i].private_handler.write(fp,arg,&parse_ele[i],NULL,(void*)tab_count);
             }
        }
        else
        {
            fprintf(fp,"%s",tabs);
            _handler[parse_ele[i].dtype].write(fp,arg,&parse_ele[i],NULL,(void*)tab_count);
        }
        i++;
    }

       return 0;
}

int _common_write_section
 (FILE* fp,char** section_name,int section_count,int found_count,void* arg,_element* parse_ele)
{
    int ret;
    auci_write_sections_name_before(fp,section_name,section_count,found_count);
    /* write the new param. */
    ret = common_write_section_cb(fp,arg,parse_ele,section_count);
    auci_write_sections_name_after(fp,section_name,section_count,found_count);
    return ret;
}


int _common_read_section(FILE* fp,void* arg,_element* parse_ele)
{
	  FILE *file = fp;
	  char buf[1024] = {0};
	  char *pEnd = NULL;
	  char* section_end;
 
	  if(!arg)
	  {
		  return -1;
	  }
      
	  while(fgets(buf,sizeof(buf),file))
	  {
		 pEnd = strchr(buf,'\n');
		 if(pEnd)
		 {
			 *pEnd = 0;
		 }
		 section_end = strchr(buf,'}');
		 if(section_end)
		 {
			 break;
		 }
		  _fill_section_cb(buf,arg,parse_ele,fp);

		  memset(buf,0,sizeof(buf));
	  }
	  

	  return 0;
 }
int auci_common_read_section 
	(char* file_name,char** section_name,int section_name_count,
	    void* section_result,_element* elements)
{
   
    FILE* fp;
	int lock_fd;
    int locate = 0;
    int ret = 9;
    
	
    if((lock_fd = __try_conf_lock_(file_name)) == -1)
    {
	    cw_logger("err __try_conf_lock_ error");
	    return -1;
    }
    __check_cfg_file_link(file_name);



    fp = fopen(file_name,"r");
    if(!fp){
        return -1;
    }
    
    locate = __locate_section_for_read(section_name, section_name_count, fp);
    //enter handler
    if(locate)
        ret = _common_read_section(fp,section_result,elements);
    else
    {
        //error 
        ret = -2;
    }
    //unlock file
    fclose(fp);
	__unlock_conf_lock_(lock_fd,file_name);
    return ret;
}

int auci_common_write_section 
	(char* file_name,char** section_name,int section_name_count,
	void* new_content,_element* elements)
{  
    char* c_file = file_name;
    char w_file[128] = {0};
    FILE* fp_r;
    FILE* fp_w;
	int lock_fd;
    int locate = 0;
    int ret = -9;

	if((lock_fd = __try_conf_lock_(file_name)) == -1)
    {
	    cw_logger("err __try_conf_lock_ error");
	    return -1;
    }

    __select_write_cfg_file(file_name,w_file);
    

    fp_r = fopen(c_file,"r");
    if(!fp_r){
        
        cw_logger("err open(r) %s",c_file);
		__unlock_conf_lock_(lock_fd,file_name);
        return -1;
    }

    fp_w = fopen(w_file,"w");
    if(!fp_w)
    {
        cw_logger("err open(w) %s",w_file);
        fclose(fp_r);
		__unlock_conf_lock_(lock_fd,file_name);
        return -2;
    }
    
	locate = __locate_section_for_write(section_name,section_name_count,fp_r,fp_w);
    //enter handler
    ret = _common_write_section(fp_w,section_name,section_name_count,locate,new_content,elements);
	__write_section_cfg_end_context(fp_r,fp_w,section_name,section_name_count,locate);

    /* write over! */
    fflush(fp_w);
    sync();

    fclose(fp_w);
    fclose(fp_r);
    
    ret = unlink(c_file);
    if(ret)
        cw_logger( 
	        "unlink %d ",ret);
    
    ret = symlink(w_file,c_file);
    if(ret)
    {
    	cw_logger(
	        "configfile is <%s> updated file <%s>," 
	        "and link it. symlink %d <%s> section name <%s>",
	        c_file,w_file,
	        ret,strerror(errno),section_name[0]);    
          
    }
	__unlock_conf_lock_(lock_fd,file_name);
    return ret;
}

