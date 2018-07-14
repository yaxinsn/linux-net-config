#ifndef _SNIFFER_SKINNY_H
#define _SNIFFER_SKINNY_H

#include <pthread.h>
#include "sniffer_lib.h"
#include "list.h"
#include "phone_session.h"
#include <time.h>

/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
/*skinny portorol  used Little-Endian */
extern FILE* skinny_log_fp;
#define SKINNY_LOG_FILE "/home/root/hzivy-skinny.log"

#define skinny_log(fmt,...)  \
    _logger_file2(skinny_log_fp,SKINNY_LOG_FILE ,__func__,__LINE__,fmt,##__VA_ARGS__);  


#define skinny_log_err(fmt,...)  \
						skinny_log("ERROR|"fmt,##__VA_ARGS__); 

enum skinny_session_state
{
        SESSION_K,
};


struct skinny_frame
{
	int from_server;// 1  yes (this is respo pkt), 0 no. 0 is to server. this is request pkt.
	char* line;
	char*  sip_msg_body;

	//struct sip_msg_header msg_hdr;
	u8 body_sdp;

	u16    rtp_port;//不知道是src dest.
	struct in_addr rtp_ip;
	//struct session_info* ss;

	enum skinny_session_state state;
};

pthread_t sniffer_skinny_start(void);


#endif //_SNIFFER_SKINNY_H

