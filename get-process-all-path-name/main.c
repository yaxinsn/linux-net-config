
#include <stdio.h>
#include <unistd.h>
#define MAXPATH 1024

#if 0
int get_all_path(const char* fileName,char* ret_path,const int size)
{

}
#endif


//获取当前程序绝对路径
int get_Self_absolute_path(char* ret_path,const int size)
{
        int cnt = readlink("/proc/self/exe", ret_path, size);
        if(cnt >0)
        {
                return 0;
        }
        else
                return -1;
}
int main(void)
{



 char buffer[MAXPATH];
if( 0 ==get_Self_absolute_path(buffer,1024))
{
 printf("The current process's file :%s\n",buffer);
}
 return 0;
}


