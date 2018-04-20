/*
	file: 电话通话的会话信息。
	author: liudan
	time: 2018-4

*/

#ifndef _PHONE_SESSION_H
#define _PHONE_SESSION_H
#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <linux/if_ether.h> //struct ethhdr

#include <netinet/in.h>
#include <netinet/ip.h> 

#include <linux/udp.h>//struct udphdr

#include <pthread.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "types_.h"
#include "list.h"

enum session_state
{
    SS_ERROR = 0,
    SS_INVATE = 1,
    SS_TRYING = 2,
    SS_RINGING = 3,
    SS_OK,
    SS_ACK,
    SS_BYE,

    
};
/* 从各种信令协议中，找出通话双方的信息，如IP，电话号码，通话时间等等。 */
struct  person
{
    struct in_addr  ip;
    u16             port;
    char*           number;
    char*           name;
};


struct session_info
{
    
    struct list_head node;
    time_t ring_time;  //响玲的时间点。
	time_t comm_time; //接通的时间点
	time_t hangup_time;//
    enum session_state state; /*从报文中同步过来。*/
    char* call_id; 
    /* 如何判断calling? 如何说INVATE的IP层srcIP == sdp.connection IP.  this ip 就
        是calling. */
    struct  person calling;
    struct  person called;
    pthread_t rtp_sniffer_tid;
    
     
}; /* 用于记录本次通信的两者的ip, port, 电话号码，用户名等等。*/


#endif //_PHONE_SESSION_H

