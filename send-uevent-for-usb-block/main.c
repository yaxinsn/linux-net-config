/*

//向/sys/block/XXX/uevent 里写add，则会重新发送netlink uevent的消息。
XXX 为usb的block。
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>


       #include <sys/types.h>
       #include <dirent.h>

#define SYSBLOCK_DIR "/sys/block/"

#define TsgErrorLog printf
#define TsgWarnLog printf
#define TsgDebugLog printf

int sendEachDiskUevent(char* blockName)
{
    char diskPath[128] = {0};
    char diskSubFileDevice[128] = {0};
    char DevicesymlinkRealFile[256] = {0};
    char DiskymlinkRealFile[256] = {0};
    char StrCmd[256];
    int ret;
    sprintf(diskPath,SYSBLOCK_DIR"%s",blockName);
    sprintf(diskSubFileDevice,"%s/device",diskPath);
    //redhat5.x
    ret = readlink(diskSubFileDevice,DevicesymlinkRealFile,256);
    if ( ret <= 0 )
    {
        TsgErrorLog("readlink (%s) error %s \n",diskSubFileDevice,strerror(errno));
    }

    if ( strstr(DevicesymlinkRealFile,"usb") )
    {
        TsgWarnLog("this block %s is usb disk\n",diskPath);
        sprintf(StrCmd,"echo add > %s/uevent",diskPath);
        system(StrCmd);
    }
    else //redhat6.x 7.x
    {

        ret = readlink(diskPath,DiskymlinkRealFile,256);
        if ( ret <= 0 )
        {
            TsgErrorLog("readlink (%s) error %s \n",diskPath,strerror(errno));
        }
        if( strstr(DiskymlinkRealFile,"usb") )
        {
            TsgWarnLog("this block %s is usb disk\n",diskPath);
            sprintf(StrCmd,"echo add > %s/uevent",diskPath);
            system(StrCmd);
        }
    }


}
int sendBlockUevent(void)
{
    DIR *d;
    struct dirent *dent;
    d = opendir(SYSBLOCK_DIR);

    if (!d)
    {
        TsgDebugLog("searchdir opendir %s failed", SYSBLOCK_DIR);
        return -1;
    }
    while ( (dent = readdir(d)) )
    {
        if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
        {
            continue;
        }

        sendEachDiskUevent(dent->d_name);

    }
    return 0;

}

int main()
{
	return 0;
}