//http://blog.csdn.net/mjt_csdn/article/details/47122189

#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include "log.h"
#include "sniffer_lib.h"

#define CAP_LEN 1024

void handle(u_char * user, const struct pcap_pkthdr * h, const u_char * p) ;




//��������ݰ����������������������ص������ĸ�ʽ�������������ʽ
void handle(u_char * user, const struct pcap_pkthdr * packet_header, const u_char * packet_content)
{
    pcap_dump(user, packet_header,packet_content);    //������ݵ��ļ�,���������ļ�����pcap��ʽ�ģ�������wireshark��
    printf("I get one packet! \n");
}

	
int get_device_info(char* device)
{
	
	char ebuf[PCAP_ERRBUF_SIZE];   //��Ŵ�����Ϣ
	bpf_u_int32 netmaskp;//�������   ��Ҫ��ת��Ϊ in_addr��ʽ�ģ�Ȼ���ٽ���inet_ntoa()ת��
    bpf_u_int32 netp;  //��������  ��Ҫ��ת��Ϊ in_addr��ʽ�ģ�Ȼ���ٽ���inet_ntoa()ת��
    char *netmask;    //���ת���������
    char *net;  //���ת����������
    struct in_addr addr;   //��Ϊת���Ĺ���
    int ret;
    ret = pcap_lookupnet(device, &netp,&netmaskp,ebuf);
    if(-1 == (ret))
        printf("pcap_net error\n");

    addr.s_addr = netp; //�ȸ�ֵ��addr
    net = inet_ntoa(addr);//��addr����ת�����ɵ��ʮ����
    printf("net is:%s %x\n",net,netp);

    addr.s_addr = netmaskp; //�ȸ�ֵ��addr
    netmask = inet_ntoa(addr);//��addr����ת�����ɵ��ʮ����
    printf("netmask is:%s %x\n",netmask,netmaskp);
	return 0;
}
/*
open_pcap_file 
device :ָ������
snaplen ���������ݰ��ĳ��ȡ�
promise : 1��ʾ�ǻ���ģʽ�������ǻ���ģʽ
to_ms   ���˾���ĵȴ�ʱ�䡣
*/
pcap_t* open_pcap_file(const char* device,int snaplen,int promise,int to_ms)
{
	char ebuf[1024];
	pcap_t *pd = NULL;    //������ݰ�����������
	int  pcap_link;  //���ڷ���������·������
	struct bpf_program fcode;
	//��ò���������
    pd = pcap_open_live(device, snaplen,promise,to_ms,ebuf);
    if(NULL == pd){
        printf("pcap_open_live error %d %s\n",errno,strerror(errno));
        exit(-1);
	}
//������·�����ͣ�ÿһ�ֶ���һ������
    pcap_link = pcap_datalink(pd);
    printf("now the datalink is:%d\n",pcap_link);

    //����ʵ��ץȡ���ַ�
    int real_cap_len = pcap_snapshot(pd);
    printf("the real cap :%d \n",real_cap_len);
	    //���ù��˹���
	
#if 1
    pcap_compile(pd, &fcode, "arp",1, 0);
    printf("pcap_compile error %d %s\n",errno,strerror(errno));
//printf("--------------\n");
    //���ù�����
    if (pcap_setfilter(pd, &fcode) < 0){
        printf("pcap_setfilter error %d %s\n",errno,strerror(errno));
    }
	printf("---------%s:%d---pd %p-\n",__func__,__LINE__,pd);
#endif    
	return   pd;     
}


int sniffer_setfilter(pcap_t * pd,const char* bpf_str)
{
	//���ù��˹���
	struct bpf_program fcode;

	//printf("---------%s:%d---pd %p--bpf_str %s\n",__func__,__LINE__,pd,bpf_str);
    if(pcap_compile(pd, &fcode, bpf_str,1, 0) <0)
    	printf("pcap_compile error %d %s\n",errno,strerror(errno));

//	log("---------%s:%d-----\n",__func__,__LINE__);
    //���ù�����
    if(pcap_setfilter(pd, &fcode) < 0)
        printf("pcap_setfilter error %d %s\n",errno,strerror(errno));
    //���ݰ�����
    return 0;
}
#if 0

void sniff_handle(u_char * user, const struct pcap_pkthdr * packet_header, const u_char * packet_content)
{
    pcap_dump(user, packet_header,packet_content);
    printf("I get one packet! \n");
}

int sniff_loop_test( pcap_t *p)
{
	//�����ڲ������ݰ����ļ�
	 const char* fname = "./icmp.pcap";
	 pcap_dumper_t *pd_t;  //�����ڱ��沶�����ݰ����ļ��� ��������ֵ
	 
	
    if( NULL == (pd_t = pcap_dump_open(p, fname) ) )
   	{
        printf("pcap_dump_open error %d %s\n",errno,strerror(errno));
    }
   
	 pcap_loop( p,10,sniff_handle,(u_char*)pd_t);
	
	 pcap_dump_flush(pd_t);
	
	 pcap_dump_close(pd_t);	 
	 return 0;
}

#endif


int check_iphdr( const struct pcap_pkthdr * phdr, const u_char * pkt,
    struct iphdr** iphdr_p )
{

	const struct ethhdr* ethh = NULL;
	const struct iphdr* iph;
	const u8*   data = pkt;
	const u16* vlan_proto = 0;
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
			if(*vlan_proto != htons(ETH_P_IP)){
				log("pkt is  vlan pkt,but not a vlanip pkt; vlan_proto %x \n",htons(*vlan_proto));
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
	if(iph->ihl <5 || iph->version != 4){
	
		log("ip header error\n");
		goto inhdr_error;
	}
	if(iph->ihl*4 >  phdr->caplen -((u8*)iph - data)){
		log("ip header len is beyond the caplen!\n");
		goto inhdr_error;
	}
	if(ntohs(iph->tot_len) > phdr->caplen -((u8*)iph - data))
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
		udph = (struct udphdr*)((u8*)iph + iph->ihl*4);
		*udph_p = udph;
		return 0;
	}
	return -1;
}