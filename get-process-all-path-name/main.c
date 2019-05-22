
#include <stdio.h>
#include <unistd.h>
#include <strings.h>


#define MAXPATH 1024
//获取当前程序绝对路径
int get_self_absolute_path(char* ret_path,const int size)
{
        int cnt = readlink("/proc/self/exe", ret_path, size);
        if(cnt >0)
        {
			return 0;
        }
        else
		{
			return -1;
		}
		
}

int get_self_absolute_father_path(char* ret_path,const int size)
{
	int iRet;
	char* p = NULL;
	iRet = get_self_absolute_path(ret_path,size);
	if(iRet == 0)
	{
		p = rindex(ret_path,'/');
		if(p == NULL)
		{
			return -1;
		}
		else
		{
#if 0
			if(p != ret_path)
			{
				*p = 0;
			}
#else
			*(p+1) = 0;		
#endif			
			return 0;
		}
	}
		
}


int main(void)
{



char buffer[MAXPATH] = {0};
if( 0 ==get_self_absolute_path(buffer,1024))
{
 printf("The current process's file :%s\n",buffer);
}


if( 0 ==get_self_absolute_father_path(buffer,1024))
{
 printf("The current process's file :%s\n",buffer);
}

 return 0;
}


