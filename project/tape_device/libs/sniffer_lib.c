//http://blog.csdn.net/mjt_csdn/article/details/47122189

#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include "log.h"
#define CAP_LEN 1024

void handle(u_char * user, const struct pcap_pkthdr * h, const u_char * p) ;




//捕获的数据包内容由这个函数处理，这个回调函数的格式必须是下面的形式
void handle(u_char * user, const struct pcap_pkthdr * packet_header, const u_char * packet_content)
{
    pcap_dump(user, packet_header,packet_content);    //输出数据到文件,输出过后的文件就是pcap格式的，可以用wireshark打开
    printf("I get one packet! \n");
}

	
int get_device_info(char* device)
{
	
	char ebuf[PCAP_ERRBUF_SIZE];   //错放错误信息
	bpf_u_int32 netmaskp;//存放掩码   需要先转换为 in_addr格式的，然后再进行inet_ntoa()转换
    bpf_u_int32 netp;  //存放网络号  需要先转换为 in_addr格式的，然后再进行inet_ntoa()转换
    char *netmask;    //存放转换后的掩码
    char *net;  //存放转换后的网络号
    struct in_addr addr;   //作为转换的过度
    int ret;
    ret = pcap_lookupnet(device, &netp,&netmaskp,ebuf);
    if(-1 == (ret))
        printf("pcap_net error\n");

    addr.s_addr = netp; //先赋值给addr
    net = inet_ntoa(addr);//将addr进行转换，成点分十进制
    printf("net is:%s %x\n",net,netp);

    addr.s_addr = netmaskp; //先赋值给addr
    netmask = inet_ntoa(addr);//将addr进行转换，成点分十进制
    printf("netmask is:%s %x\n",netmask,netmaskp);
	return 0;
}
/*
open_pcap_file 
device :指定网卡
snaplen ：捕获数据包的长度。
promise : 1表示是混杂模式，其它非混杂模式
to_ms   ：此句柄的等待时间。
*/
pcap_t* open_pcap_file(char* device,int snaplen,int promise,int to_ms)
{
	char ebuf[1024];
	pcap_t *pd = NULL;    //存放数据包捕获描述字
	int  pcap_link;  //用于返回数据链路层类型
	struct bpf_program fcode;
	//获得捕获描述字
    pd = pcap_open_live(device, snaplen,promise,to_ms,ebuf);
    if(NULL == pd){
        printf("pcap_open_live error %d %s\n",errno,strerror(errno));
        exit(-1);
	}
//返回链路层类型，每一种都有一个代号
    pcap_link = pcap_datalink(pd);
    printf("now the datalink is:%d\n",pcap_link);

    //返回实际抓取的字符
    int real_cap_len = pcap_snapshot(pd);
    printf("the real cap :%d \n",real_cap_len);
	    //设置过滤规则
	
#if 1
    pcap_compile(pd, &fcode, "arp",1, 0);
    printf("pcap_compile error %d %s\n",errno,strerror(errno));
//printf("--------------\n");
    //设置过滤器
    if (pcap_setfilter(pd, &fcode) < 0){
        printf("pcap_setfilter error %d %s\n",errno,strerror(errno));
    }
	printf("---------%s:%d---pd %p-\n",__func__,__LINE__,pd);
#endif    
	return   pd;     
}


int sniffer_setfilter(pcap_t * pd,const char* bpf_str)
{
	    //设置过滤规则
	struct bpf_program fcode;

	printf("---------%s:%d---pd %p--bpf_str %s\n",__func__,__LINE__,pd,bpf_str);
    if(pcap_compile(pd, &fcode, bpf_str,1, 0) <0)
    	printf("pcap_compile error %d %s\n",errno,strerror(errno));

	log("---------%s:%d-----\n",__func__,__LINE__);
    //设置过滤器
    if(pcap_setfilter(pd, &fcode) < 0)
        printf("pcap_setfilter error %d %s\n",errno,strerror(errno));
    //数据包捕获
    return 0;
}


void sniff_handle(u_char * user, const struct pcap_pkthdr * packet_header, const u_char * packet_content)
{
    pcap_dump(user, packet_header,packet_content);
    printf("I get one packet! \n");
}

int sniff_loop_test( pcap_t *p)
{
	//打开用于捕获数据包的文件
	char* fname = "./icmp.pcap";
	 pcap_dumper_t *pd_t;  //打开用于保存捕获数据包的文件， 函数返回值
	 
	
    if( NULL == (pd_t = pcap_dump_open(p, fname) ) )
   	{
        printf("pcap_dump_open error %d %s\n",errno,strerror(errno));
    }
   
	 pcap_loop( p,10,sniff_handle,(u_char*)pd_t);
	
	 pcap_dump_flush(pd_t);
	
	 pcap_dump_close(pd_t);	 
	 return 0;
}