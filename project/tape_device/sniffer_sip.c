

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
#include <arpa/inet.h>
#include "log.h"


/*
sniffer_handle_sip 一个包一个包地进行处理。
每抓一个包被调用一次。

*/
static void sniffer_handle_sip(u_char * user, const struct pcap_pkthdr * packet_header, const u_char * packet_content)
{
    
    printf("I get one packet! \n");
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

void sniffer_sip_start()
{

}


