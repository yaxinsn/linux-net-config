

/******************************************
*
* ���ļ��ĳ�����Ҫ������ץ SIP�ȱ��ġ�
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
sniffer_handle_sip һ����һ�����ؽ��д���
ÿץһ����������һ�Ρ�

*/
static void sniffer_handle_sip(u_char * user, const struct pcap_pkthdr * packet_header, const u_char * packet_content)
{
    
    printf("I get one packet! \n");
}

/*
����������ץ���Ĺ��̣�һ��ֻץ sip, ��һ��ץRTP
�������Ҫ�����̡߳�һ���߳��ǳ�פ������ץSIP 
��һ���߳��Ƕ�̬�ģ�����ץRTP.

*/
int sniffer_loop_sip( pcap_t *p)
{
	 pcap_loop( p,-1,sniffer_handle_sip,(u_char*)NULL);
	 return 0;
}

void sniffer_sip_start()
{

}


