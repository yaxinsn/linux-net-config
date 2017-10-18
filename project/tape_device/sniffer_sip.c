

/******************************************
*
* 本文件的程序主要是用来抓 SIP等报文。
* 
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "log.h"
#include <arpa/inet.h>

#include <linux/if_ether.h> //struct ethhdr
#include <linux/ip.h> //struct iphdr
#include <linux/udp.h>//struct udphdr
//#include <linux/in.h>

/*
sniffer_handle_sip 一个包一个包地进行处理。
每抓一个包被调用一次。

struct pcap_pkthdr  
{  
    struct timeval ts; // 抓到包的时间  
    bpf_u_int32 caplen; // 表示抓到的数据长度  
    bpf_u_int32 len; // 表示数据包的实际长度  
}  


*/
/****************************************** check and parse **************************************/
int check_iphdr( const struct pcap_pkthdr * phdr, const u_char * pkt,struct iphdr** iphdr_p )
{

	const struct ethhdr* ethh = NULL;
	const struct iphdr* iph;
	u8*   data = pkt;
	u16* vlan_proto = 0;
	if (phdr->len != phdr->caplen)
	{
		log(" phdr->len %d != phdr->caplen %d \n",phdr->len,phdr->caplen);
		return -1;
	}
	ethh =(struct ethhdr*) data;
	if(ethh->h_proto != htons(ETH_P_IP))
	{
		if(ethh->h_proto != htons(ETH_P_8021Q))
		{
			vlan_proto = &ethh->h_proto;
			vlan_proto += 2;// shift 4 byte;
			if(vlan_proto != htons(ETH_P_IP)){
				log("pkt is  vlan pkt,but not a vlanip pkt; vlan_proto %x \n",htons(vlan_proto));
				return -1;
			}
			else
				iph = (struct iphdr*)vlan_proto+1;
		}
		else
		{
			log("pkt is not ip or vlan pkt; ethh->h_proto %x \n",htons(ethh->h_proto));
			return -1;
		}
	}
	else
	{
		iph = (struct iphdr*)(ethh+1);
	}
	/* ip hdr is corrent */
	if(iph->ihl <5 || iph->version != 4)
		goto inhdr_error;
	if(iph->ihl*4 >  phdr->caplen -(iph - data)){
		log("ip header len is beyond the caplen!\n");
		goto inhdr_error;
	}
	if(ntohs(iph->tot_len > phdr->caplen -(iph - data))
	{
		log(" ip tot len is beyond the caplen! \n");
		goto inhdr_error;
	}
	*iphdr_p = iph;
	
	return 0;
inhdr_error:
	return -2;
}
int check_udp( struct iphdr* iph,struct udphdr** udph_p)
{
	struct udphdr* udph;
	if(iph->protocol == IPPROTO_UDP)
	{
		udph = (struct udphdr*)((u8*)iph + iph->iph*4);
		*udph_p = udph;
		return 0;
	}
	return -1;
}
// SIP包内容标记
#define SIPTAGANDVERSION1 "SIP/2.0"		// 回应, 在第一行开始位置
#define SIPTAGANDVERSION2 " SIP/2.0"	// 请求, 在第一行结束位置
// SIP包结束标记
#define SIPPACKENDTAG "\r\n\r\n"
#define SIPSDPPACKENDTAG "\r\n"

/* 在一行的最后加个\0,\r\n-->\0 然后返回下一行。 */
char* split_line_next(char* l)
{
	char* next = strstr(sip,SIPSDPPACKENDTAG);
	if(next == NULL)
		return next;
	next[0] = 0;
	next+=2;
	return next;
}


int check_sip( struct udphdr* udph)
{
	//char* sip = (u8*)(udph+1);
	char* request_line;
	char* mesg_header;
	char* mesg_body;
	char* sip = malloc(htons(udph->len));
	if(sip == NULL)
		return -1;
	memcpy(sip,(u8*)(udph+1),htons(udph->len)-8);
	
	char* request_line = sip;
	char* mesg_header = split_line_next(sip);
	if(
	if(mesg_header != NULL && (mesg_header[0] == '\r')&&(mesg_header[1] == '\n'))
	{
		if
		mesg_header+=2;
	}
	
	char* mesg_body;
	
}
/***********************************************************************************/
static void sniffer_handle_sip(u_char * user, const struct pcap_pkthdr * packet_header, const u_char * packet_content)
{
    int ret = 0;
	struct pcap_pkthdr* phdr = packet_header;
	struct iphdr* iph = NULL;
	struct udphdr* udph = NULL;
	
    printf("I get one packet! \n");
	ret = check_iphdr(phdr,packet_content,&iph);
	if(ret != 0)
		goto error;
	
	if(0 != check_udp(iph,&udph))
		goto error;
	
	
		
	}
error:

}

/*
这里有两个抓包的过程，一是只抓 sip, 另一个抓RTP
这里就需要两个线程。一个线程是常驻，用于抓SIP 
另一个线程是动态的，用于抓RTP.

*/
int sniffer_loop_sip( pcap_t *p)
{

	

	 pcap_loop( p,-1,sniffer_handle_sip,(u_char*)NULL);
	 return 0;
}

void* sniffer_sip_loop(void* arg)
{

	char filter[200] = {0};

	pcap_t* pd = open_pcap_file("br-wan",65535,1,0);
	if(pd == NULL)
	{

		log("open_pcap_file failed ! \n");
		exit(1);
	}

	sprintf(filter,"udp and host %s and port %d ",inet_ntoa(g_config.call.ip),g_config.call.port);
	sniffer_setfilter(pd,filter);
	sniff_loop_test(pd);
	while(1)
	{
		sniffer_loop_sip(pd);
	}
}


pthread_t __sniffer_sip_start(void)
{
	pthread_t tid;
	
	if(pthread_create(&tid,NULL,sniffer_sip_loop,NULL))
	{
		log("create msg_engine_start sniffer_sip_loop failed\n");
		return -1;
	}

	return tid;

}
void sniff_handle(u_char * user, const struct pcap_pkthdr * packet_header, const u_char * packet_content)
{
    pcap_dump(user, packet_header,packet_content);
    printf("I get one packet! \n");
}

int __sniffer_init()
{

}

pthread_t sniffer_sip_start(void)
{
	pthread_t tid;
	if(__sniffer_init() != 0)
	{
		log("sniffer failed ,so program exit!\n");
		exit(1);
	}
	tid = __sniffer_sip_start();
	return tid;

}

