

#ifndef UPLOAD_H
#define UPLOAD_H

#include <stdio.h>
#include <unistd.h>
#include <sys/un.h> 
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/queue.h>
#include <pthread.h>
#include "types_.h"

/* sniffer与uploader之间的消息定义 */

enum PHONE_EVENT{
	RING_UP = 1, /* 打电话 */
	TALKING, /* 通话 */
	RING_DOWN, /* 挂电话 */
	SIP_PKT
};

struct phone_msg 
{
	enum PHONE_EVENT event;
	u8 data[0];
	
};

struct talking_mesg
{
	u32 resv;
	u32 phone_sender_ip;
	u16 phone_sender_port;
	u16 resv1;
	
	u32 phone_rcv_ip;
	u16 phone_rcv_port;
	u8  proto;
	u8  resv2;
	
	u16 lenth;
	u8 data[0];
};


pthread_t uploader_start(void);
int uploader_push_msg(struct phone_msg*  msg,int len);


#endif

