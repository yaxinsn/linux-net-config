
#ifndef SNIFFER_LIB_H
#define SNIFFER_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include "log.h"

int get_device_info(char* device);
pcap_t* open_pcap_file(char* device,int snaplen,int promise,int to_ms);
int sniffer_setfilter(pcap_t * pd,const char* bpf_str);
pcap_handler sniff_handle(u_char * user, 
	const struct pcap_pkthdr * packet_header, const u_char * packet_content);








#endif

