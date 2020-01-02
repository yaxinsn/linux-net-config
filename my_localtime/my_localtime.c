/*

localtime_r是线程安全的，但是，对如下两种情况并不安全，甚至会引发死锁。

（1）信号处理函数调用localtime：假如进程调用localtime，已经获取全局锁，且并没有释放。此时，如果进程接收到信号，在信号处理函数也调用了localtime，就会造成死锁。

（2）多线程下fork：在多线程下，若线程A调用localtime，已经获取全局锁，尚未释放锁。此时，假如线程B调用了fork，并且在子进程也调用localtime，也会造成死锁，导致子进程一直被hang住。因为fork出来的子进程只会复制调用它的线程，而其他线程不会被复制到子进程执行，也就是说当前子进程中只有线程B在运行。子进程会复制父进程的用户空间数据，包括了锁的信息。
————————————————
版权声明：本文为CSDN博主「pengzhouzhou」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/pengzhouzhou/article/details/87095635

在此自己写一个
*/



#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <unistd.h>
#define LEAPS_THRU_END_OF(y) ((y)/4 - (y)/100 + (y)/400)
#define LEAP_YEAR(year) ((!(year % 4) && (year % 100)) || !(year % 400))
/*
 *  * Convert seconds since 01-01-1970 00:00:00 to Gregorian date.
 *   */
    static const unsigned char rtc_days_in_month[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

 int rtc_month_days(unsigned int month, unsigned int year)
{
	return rtc_days_in_month[month] + (LEAP_YEAR(year) && month == 1);
}

void time_to_tm(unsigned long time, struct tm *tm)
{
	unsigned int month, year;
	int days;

	days = time / 86400;
	time -= (unsigned int) days * 86400;

	/* day of the week, 1970-01-01 was a Thursday */
	tm->tm_wday = (days + 4) % 7;

	year = 1970 + days / 365;
	days -= (year - 1970) * 365
		+ LEAPS_THRU_END_OF(year - 1)
		- LEAPS_THRU_END_OF(1970 - 1);
	if (days < 0) {
		year -= 1;
		days += 365 + LEAP_YEAR(year);
	}
	tm->tm_year = year - 1900;
	tm->tm_yday = days + 1;

	for (month = 0; month < 11; month++) {
		int newdays;

		newdays = days - rtc_month_days(month, year);
		if (newdays < 0)
			break;
		days = newdays;
	}
	tm->tm_mon = month;
	tm->tm_mday = days + 1;

	tm->tm_hour = time / 3600;
	time -= tm->tm_hour * 3600;
	tm->tm_min = time / 60;
	tm->tm_sec = time - tm->tm_min * 60;
	return;
}
static void __local_time_str(char* time_str,int len)
{


	time_t a;
	struct tm t;

	struct timeval  tv;
	struct timezone  tz;
	memset(&tz,0x55,sizeof(tz));
	gettimeofday (&tv,&tz);
	a = tv.tv_sec - tz.tz_minuteswest*60;
	time_to_tm(a, &t);
	strftime(time_str,len,"%Y-%m-%d_%H-%M-%S",&t);
}
int main()
{
#if 1	
	struct timeval  tv;
	struct timezone  tz;
	gettimeofday (&tv,&tz);
	printf("tv_sec; %d\n", tv.tv_sec) ;
	printf("tv_usec; %d\n",tv.tv_usec);
	printf("tz_minuteswest; %d\n", tz.tz_minuteswest);
	printf("tz_dsttime, %d\n",tz.tz_dsttime);
	printf("##### time %u \n",time(NULL));
#endif

	{
	char time_str[64] = {0};
	__local_time_str(time_str,64);
	printf("$$$$$$ time %s \n",time_str);
	}
	return 0;
}
