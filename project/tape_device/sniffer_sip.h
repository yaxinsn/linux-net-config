
#ifndef SNIFFER_SIP_H

#define SNIFFER_SIP_H 
	
#include "sniffer_lib.h"



struct sip_msg_header
{
	char* contant;
	char* expires;
	char* to;
	char* via;
	char* from;
	char* cseq;
	char* user_agent;
	char* Max_forwards;
	char* allow;
	int content_length;
	   
	
};
enum REQ_TYPE
{
	REQ_REG = 1,
	REQ_INVITE,
	REQ_ACK,
	REQ_BYE,
	REQ_OPTIONS,
	REQ_CANCEL,

	
};
enum RES_TYPE
{
RES_TRYING = 1,
RES_UNAUTH,
RES_OK,
RES_SERVER_UNAVAI,
RES_RINGING,

};
struct sip_pkt
{
	int type;// 1 request 2 response
	enum REQ_TYPE request_type; //  
	enum RES_TYPE response_type;
	char* line;
	struct sip_msg_header msg_hdr;
	
	int body_sdp;
	char*  sip_msg_body;
	u16    rtp_port;//2??a¦Ì¨¤¨º?src dest.
};

#include <pthread.h>

pthread_t sniffer_sip_start();

#endif

