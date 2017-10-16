需要在此目录下有一个json-c的源代码目录。并且编译完成。
main.c里需要它的头文件和*.so



编译json-c-0.12.的命令

在Makefile找到CFLAGS = -g -O2 ,在它的后面的增加-Wno-error=unused-but-set-variable
CFLAGS = -g -O2 -Wno-error=unused-but-set-variable
再进行make