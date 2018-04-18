
#ifndef SNIFFER_SIP_H

#define SNIFFER_SIP_H 
	
#include <pthread.h>
#include "sniffer_lib.h"
#include "list.h"
#include "phone_session.h"
#include <time.h>


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


struct sip_msg_header
{
	char* contant;
	char* expires;
	char* to;
	char* call_id;
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
struct session_info;

struct sip_pkt
{
	int type;// 1 request 2 response
	enum REQ_TYPE request_type; //  
	enum RES_TYPE response_type;
	char* line;
	char*  sip_msg_body;
	
	struct sip_msg_header msg_hdr;
	u8 body_sdp;
	
	u16    rtp_port;//不知道是src dest.
    struct in_addr rtp_ip;
	//struct session_info* ss;
	
    enum session_state state;
};



pthread_t sniffer_sip_start();

#endif

