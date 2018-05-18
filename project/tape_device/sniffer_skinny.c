
#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "log.h"

#include <pthread.h>    

#include "sniffer_lib.h"  


#include "config.h"
#include "linux-utils.h"
#include "wake_utils.h"
#include "sniffer_skinny.h"
#include "str_lib.h"
#include "upload.h"

#include "sniffer_rtp.h"

extern struct config_st g_config;



#define TCP_PORT_SKINNY 2000 /* Not IANA registered */

#define SSL_PORT_SKINNY 2443 /* IANA assigned to PowerClient Central Storage Facility */

#define BASIC_MSG_TYPE 0x00
#define V10_MSG_TYPE 0x0A
#define V11_MSG_TYPE 0x0B
#define V15_MSG_TYPE 0x0F
#define V16_MSG_TYPE 0x10
#define V17_MSG_TYPE 0x11
#define V18_MSG_TYPE 0x12
#define V19_MSG_TYPE 0x13
#define V20_MSG_TYPE 0x14
#define V21_MSG_TYPE 0x15
#define V22_MSG_TYPE 0x16

typedef struct _value_string {
    unsigned int      value;
    const char		 *strptr;
} value_string;


static const value_string header_version[] = {
  { BASIC_MSG_TYPE, "Basic" },
  { V10_MSG_TYPE,   "V10" },
  { V11_MSG_TYPE,   "V11" },
  { V15_MSG_TYPE,   "V15" },
  { V16_MSG_TYPE,   "V16" },
  { V17_MSG_TYPE,   "V17" },
  { V18_MSG_TYPE,   "V18" },
  { V19_MSG_TYPE,   "V19" },
  { V20_MSG_TYPE,   "V20" },
  { V21_MSG_TYPE,   "V21" },
  { V22_MSG_TYPE,   "V22" },
  { 0             , NULL }
};

/* Declare MessageId */
static const value_string message_id[] = {
  { 0x0000, "KeepAliveReq" },
  { 0x0001, "RegisterReq" },
  { 0x0002, "IpPort" },
  { 0x0003, "KeypadButton" },
  { 0x0004, "EnblocCall" },
  { 0x0005, "Stimulus" },
  { 0x0006, "OffHook" },
  { 0x0007, "OnHook" },
  { 0x0008, "HookFlash" },
  { 0x0009, "ForwardStatReq" },
  { 0x000a, "SpeedDialStatReq" },
  { 0x000b, "LineStatReq" },
  { 0x000c, "ConfigStatReq" },
  { 0x000d, "TimeDateReq" },
  { 0x000e, "ButtonTemplateReq" },
  { 0x000f, "VersionReq" },
  { 0x0010, "CapabilitiesRes" },
  { 0x0012, "ServerReq" },
  { 0x0020, "Alarm" },
  { 0x0021, "MulticastMediaReceptionAck" },
  { 0x0022, "OpenReceiveChannelAck" },
  { 0x0023, "ConnectionStatisticsRes" },
  { 0x0024, "OffHookWithCalingPartyNumber" },
  { 0x0025, "SoftKeySetReq" },
  { 0x0026, "SoftKeyEvent" },
  { 0x0027, "UnregisterReq" },
  { 0x0028, "SoftKeyTemplateReq" },
  { 0x0029, "RegisterTokenReq" },
  { 0x002a, "MediaTransmissionFailure" },
  { 0x002b, "HeadsetStatus" },
  { 0x002c, "MediaResourceNotification" },
  { 0x002d, "RegisterAvailableLines" },
  { 0x002e, "DeviceToUserData" },
  { 0x002f, "DeviceToUserDataResponse" },
  { 0x0030, "UpdateCapabilities" },
  { 0x0031, "OpenMultiMediaReceiveChannelAck" },
  { 0x0032, "ClearConference" },
  { 0x0033, "ServiceURLStatReq" },
  { 0x0034, "FeatureStatReq" },
  { 0x0035, "CreateConferenceRes" },
  { 0x0036, "DeleteConferenceRes" },
  { 0x0037, "ModifyConferenceRes" },
  { 0x0038, "AddParticipantRes" },
  { 0x0039, "AuditConferenceRes" },
  { 0x0040, "AuditParticipantRes" },
  { 0x0041, "DeviceToUserDataVersion1" },
  { 0x0042, "DeviceToUserDataResponseVersion1" },
  { 0x0043, "CapabilitiesV2Res" },
  { 0x0044, "CapabilitiesV3Res" },
  { 0x0045, "PortRes" },
  { 0x0046, "QoSResvNotify" },
  { 0x0047, "QoSErrorNotify" },
  { 0x0048, "SubscriptionStatReq" },
  { 0x0049, "MediaPathEvent" },
  { 0x004a, "MediaPathCapability" },
  { 0x004c, "MwiNotification" },
  { 0x0081, "RegisterAck" },
  { 0x0082, "StartTone" },
  { 0x0083, "StopTone" },
  { 0x0085, "SetRinger" },
  { 0x0086, "SetLamp" },
  { 0x0087, "SetHookFlashDetect" },
  { 0x0088, "SetSpeakerMode" },
  { 0x0089, "SetMicroMode" },
  { 0x008a, "StartMediaTransmission" },
  { 0x008b, "StopMediaTransmission" },
  { 0x008f, "CallInfo" },
  { 0x0090, "ForwardStatRes" },
  { 0x0091, "SpeedDialStatRes" },
  { 0x0092, "LineStatRes" },
  { 0x0093, "ConfigStatRes" },
  { 0x0094, "TimeDateRes" },//---------liudan å–å¾—æ—¶é—´ã€‚
  { 0x0095, "StartSessionTransmission" },
  { 0x0096, "StopSessionTransmission" },
  { 0x0097, "ButtonTemplateRes" },
  { 0x0098, "VersionRes" },
  { 0x0099, "DisplayText" },
  { 0x009a, "ClearDisplay" },
  { 0x009b, "CapabilitiesReq" },
  { 0x009d, "RegisterReject" },
  { 0x009e, "ServerRes" },
  { 0x009f, "Reset" },
  { 0x0100, "KeepAliveAck" },
  { 0x0101, "StartMulticastMediaReception" },
  { 0x0102, "StartMulticastMediaTransmission" },
  { 0x0103, "StopMulticastMediaReception" },
  { 0x0104, "StopMulticastMediaTransmission" },
  { 0x0105, "OpenReceiveChannel" },
  { 0x0106, "CloseReceiveChannel" },
  { 0x0107, "ConnectionStatisticsReq" },
  { 0x0108, "SoftKeyTemplateRes" },
  { 0x0109, "SoftKeySetRes" },
  { 0x0110, "SelectSoftKeys" },
  { 0x0111, "CallState" },
  { 0x0112, "DisplayPromptStatus" },
  { 0x0113, "ClearPromptStatus" },
  { 0x0114, "DisplayNotify" },
  { 0x0115, "ClearNotify" },
  { 0x0116, "ActivateCallPlane" },
  { 0x0117, "DeactivateCallPlane" },
  { 0x0118, "UnregisterAck" },
  { 0x0119, "BackSpaceRes" },
  { 0x011a, "RegisterTokenAck" },
  { 0x011b, "RegisterTokenReject" },
  { 0x011c, "StartMediaFailureDetection" },
  { 0x011d, "DialedNumber" },
  { 0x011e, "UserToDeviceData" },
  { 0x011f, "FeatureStatRes" },
  { 0x0120, "DisplayPriNotify" },
  { 0x0121, "ClearPriNotify" },
  { 0x0122, "StartAnnouncement" },
  { 0x0123, "StopAnnouncement" },
  { 0x0124, "AnnouncementFinish" },
  { 0x0127, "NotifyDtmfTone" },
  { 0x0128, "SendDtmfTone" },
  { 0x0129, "SubscribeDtmfPayloadReq" },
  { 0x012a, "SubscribeDtmfPayloadRes" },
  { 0x012b, "SubscribeDtmfPayloadErr" },
  { 0x012c, "UnSubscribeDtmfPayloadReq" },
  { 0x012d, "UnSubscribeDtmfPayloadRes" },
  { 0x012e, "UnSubscribeDtmfPayloadErr" },
  { 0x012f, "ServiceURLStatRes" },
  { 0x0130, "CallSelectStatRes" },
  { 0x0131, "OpenMultiMediaReceiveChannel" },
  { 0x0132, "StartMultiMediaTransmission" },
  { 0x0133, "StopMultiMediaTransmission" },
  { 0x0134, "MiscellaneousCommand" },
  { 0x0135, "FlowControlCommand" },
  { 0x0136, "CloseMultiMediaReceiveChannel" },
  { 0x0137, "CreateConferenceReq" },
  { 0x0138, "DeleteConferenceReq" },
  { 0x0139, "ModifyConferenceReq" },
  { 0x013a, "AddParticipantReq" },
  { 0x013b, "DropParticipantReq" },
  { 0x013c, "AuditConferenceReq" },
  { 0x013d, "AuditParticipantReq" },
  { 0x013e, "ChangeParticipantReq" },
  { 0x013f, "UserToDeviceDataVersion1" },
  { 0x0140, "VideoDisplayCommand" },
  { 0x0141, "FlowControlNotify" },
  { 0x0142, "ConfigStatV2Res" },
  { 0x0143, "DisplayNotifyV2" },
  { 0x0144, "DisplayPriNotifyV2" },
  { 0x0145, "DisplayPromptStatusV2" },
  { 0x0146, "FeatureStatV2Res" },
  { 0x0147, "LineStatV2Res" },
  { 0x0148, "ServiceURLStatV2Res" },
  { 0x0149, "SpeedDialStatV2Res" },
  { 0x014a, "CallInfoV2" },
  { 0x014b, "PortReq" },
  { 0x014c, "PortClose" },
  { 0x014d, "QoSListen" },
  { 0x014e, "QoSPath" },
  { 0x014f, "QoSTeardown" },
  { 0x0150, "UpdateDSCP" },
  { 0x0151, "QoSModify" },
  { 0x0152, "SubscriptionStatRes" },
  { 0x0153, "Notification" },
  { 0x0154, "StartMediaTransmissionAck" },
  { 0x0155, "StartMultiMediaTransmissionAck" },
  { 0x0156, "CallHistoryInfo" },
  { 0x0157, "LocationInfo" },
  { 0x0158, "MwiRes" },
  { 0x0159, "AddOnDeviceCapabilities" },
  { 0x015a, "EnhancedAlarm" },
  { 0x015e, "CallCountReq" },
  { 0x015f, "CallCountResp" },
  { 0x0160, "RecordingStatus" },
  { 0x8000, "SPCPRegisterTokenReq" },
  { 0x8100, "SPCPRegisterTokenAck" },
  { 0x8101, "SPCPRegisterTokenReject" },
  {0     , NULL}
};

typedef struct skinny_packet_info
{
    u32 callid;
    
}skinny_info_t;

typedef struct _skinny_opcode_map_t skinny_opcode_map_t;

/* begin conversaton  info*/
typedef enum _skinny_message_type_t {
  SKINNY_MSGTYPE_EVENT    = 0,
  SKINNY_MSGTYPE_REQUEST  = 1,
  SKINNY_MSGTYPE_RESPONSE = 2,
} skinny_message_type_t;

  

typedef void (*message_handler) (skinny_opcode_map_t* skinny_op, 
                                    u8* msg,u32 len,skinny_info_t* skinny);

typedef struct _skinny_opcode_map_t {
  u32 opcode;
  message_handler handler;
  skinny_message_type_t type;
  const char *name;
} skinny_opcode_map_t;




/*******************************************************************************************************/


/******************************************
*
* æœ¬æ–‡ä»¶çš„ç¨‹åºä¸»è¦æ˜¯ç”¨æ¥æŠ“ SKINNY æŠ¥æ–‡ã€‚
* æœ¬ç¨‹åºæŠ“sipçš„æŠ¥æ–‡ï¼Œå¹¶è§£æžå‡ºsipé‡Œçš„call-id,ä»¥æ­¤ä½œä¸ºkey,æŠŠpktçš„è¿‡ç¨‹æ”¾åˆ°
ä¸€ä¸ªsessionä¸­ï¼Œsessionæ”¾åˆ°ä¸€ä¸ªå…¨å±€çš„é“¾è¡¨ä¸­ã€‚
è·Ÿè¸ªæ¯ä¸ªsessionçš„ä»Žç”Ÿåˆ°æ­»çš„è¿‡ç¨‹ã€‚æœ€åŽfree session.

* 
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "log.h"

#include <pthread.h>    

#include "sniffer_lib.h"  


#include "config.h"
#include "linux-utils.h"
#include "wake_utils.h"
#include "sniffer_sip.h"
#include "str_lib.h"
#include "upload.h"


extern struct config_st g_config;



struct skinny_frame_info {
	u32 callRef;//callid;
};


#include "sniffer_skinny.h"

typedef unsigned int UINT;
typedef unsigned short USHORT;

#define CW_LOAD_U8(ucValue, pucBuf)((ucValue) = (*(pucBuf)++));

#define CW_LOAD_U16(usValue, pucBuf)\
{\
    usValue  = (USHORT) ((*(pucBuf)++) );\
    usValue |= (USHORT) ((*(pucBuf)++)<< 8);\
}

#define CW_LOAD_U32(uiValue, pucBuf)\
{\
    uiValue  = (UINT) ((*(pucBuf)++));\
    uiValue |= (UINT) ((*(pucBuf)++) << 8);\
    uiValue |= (UINT) ((*(pucBuf)++) << 16);\
    uiValue |= (UINT) ((*(pucBuf)++) << 24);\
}


#define CW_LOAD_STR(pucDest, pucBuf, ulLen)\
{\
    memcpy((pucDest), (pucBuf), (ulLen));\
    ((pucBuf) += (ulLen));\
}


struct session_info* skinny_get_session(char* callid)
{
	struct session_info* ss=si_find_session(callid);
	if(ss == NULL)
	{
	    return NULL;
	    #if 0
		ss = si_new_session();
		if(ss ==NULL)
			return NULL;
		ss->call_id = strdup(callid);
		if(ss->call_id == NULL){
			si_del_session(ss);
			return NULL;
		}
		#endif
	}
	return ss;
}
static  struct session_info* skinny_get_session_by_callRef(u32 callReference)
{
	
	struct session_info* ss;
	char callid[64]={0};
	
	sprintf(callid,"%d",callReference);
	    
	ss = skinny_get_session(callid);
	if(ss ==  NULL)
	{
		skinny_log_err("no this callid %s session\n",callid);
	}
	return ss;
}
/*------------------------------------------------*/
typedef struct qualifierOut_st
{
    u32 percedenceValue;
    u32 ssValue;
    u32 maxFramesPerpacket;
    u32 any_compressionType;
}qualifierOut;
/* all session 's end is clearPromptStatus 2018-5-6 */
void handle_clear_prompt_status(skinny_opcode_map_t* skinny_op, 
                    u8* msg,u32 len,
                    skinny_info_t* skinny_info)
{
    u8* p = msg;
    u32 lineInstance;
    u32 callRefer;
    
    CW_LOAD_U32(lineInstance,p);
    CW_LOAD_U32(callRefer,p);
    skinny_log("enter\n");
    skinny_info->callid = callRefer;
    struct session_info* ss;
    ss = skinny_get_session_by_callRef(callRefer);
    if(ss)
        close_rtp_sniffer(ss);
    else
        skinny_log(" Not find this callrefer %d  \n",callRefer);
}
#if 0
void handle_stop_media_transmission(skinny_opcode_map_t* skinny_op, 
                    u8* msg,u32 len,
                    skinny_info_t* skinny_info)
{
    u8* p = msg;
    u32 conferenceID;
    u32 passThruPartyID;
    u32 callRefer;
    u32 portHandlingFlag;
    
    CW_LOAD_U32(conferenceID,p);
    CW_LOAD_U32(passThruPartyID,p);
    CW_LOAD_U32(callRefer,p);
    CW_LOAD_U32(portHandlingFlag,p);

    struct session_info* ss;
    ss = skinny_get_session_by_callRef(callRefer);
    if(ss)
        close_rtp_sniffer(ss);
    else
        skinny_log(" Not find this callrefer %d  \n",callRefer);
}

#endif

/* center nofity peer's ip+port to csico phone */

void handle_start_media_transmission(skinny_opcode_map_t* skinny_op, u8* msg,u32 len,
                    skinny_info_t* skinny_info)
{
    u8* p = msg;
    u32 conferenceID;
    u32 status;
    u32 ipv4orv6;
    u32 remoteIpv4Address;
    u32 remotePort;
    u32 millisecondPaccketSize;
    u32 passThruPartyID;
    u32 compressionType;
    u32 callRefer;
    qualifierOut qualifier_out;
//    u32 callRefer;
    
    struct session_info* ss;
    skinny_log("enter\n");
    
    CW_LOAD_U32(conferenceID,p);
    CW_LOAD_U32(passThruPartyID,p);
    CW_LOAD_U32(ipv4orv6,p);
    CW_LOAD_U32(remoteIpv4Address,p);
    p+=12;
    CW_LOAD_U32(remotePort,p);
    CW_LOAD_U32(millisecondPaccketSize,p);
    
    CW_LOAD_U32(compressionType,p);

    CW_LOAD_U32(qualifier_out.percedenceValue,p);
    CW_LOAD_U32(qualifier_out.ssValue,p);
    CW_LOAD_U32(qualifier_out.maxFramesPerpacket,p);
    CW_LOAD_U32(qualifier_out.any_compressionType,p);
    
    CW_LOAD_U32(callRefer,p);
    skinny_log("_-_-_----------- callrefer %d , Port %d  any_compressionType %x \n",
        callRefer,remotePort,qualifier_out.any_compressionType);

    skinny_info->callid = callRefer;

    ss = skinny_get_session_by_callRef(callRefer);
	if(ss)
	{
	    if(ss->mode == SS_MODE_CALLING){
	        ss->called.ip.s_addr = remoteIpv4Address;
	        ss->called.port = (remotePort);
	    }
	    else
	    {
	        ss->calling.ip.s_addr = remoteIpv4Address;
	        ss->calling.port = (remotePort);
	    }
	    ss->rtp_sniffer_tid = setup_rtp_sniffer(ss);
	}
	
}

/* Ë¼¿Æ»°»ú¸æËßcenter ×Ô¼ºµÄip+port. */
void handle_open_receive_channel_ack(skinny_opcode_map_t* skinny_op, u8* msg,u32 len,
                    skinny_info_t* skinny_info)
{
    u8* p = msg;
    u32 openRecvChannelstatus;
    u32 ipv4orv6;
    u32 ipv4Address;
    u32 Port;
    /* ext info:  */
    u32 passThruPartyID;
    u32 callRefer;
    struct session_info* ss;
    
    skinny_log("enter\n");
    CW_LOAD_U32(openRecvChannelstatus,p);
    CW_LOAD_U32(ipv4orv6,p);
    CW_LOAD_U32(ipv4Address,p);
    p+=12;
    CW_LOAD_U32(Port,p);
    CW_LOAD_U32(passThruPartyID,p);
    CW_LOAD_U32(callRefer,p);
    skinny_log("_-_-_----------- callrefer %d , Port %d ipv4Address %d \n",
    callRefer,Port,ipv4Address);
    
    skinny_info->callid = callRefer;
	ss = skinny_get_session_by_callRef(callRefer);
	if(ss)
	{
	    if(ss->mode == SS_MODE_CALLING){
	        ss->calling.ip.s_addr = ipv4Address;
	        ss->calling.port = (Port);
	    }
	    else
	    {
	        
	        ss->called.ip.s_addr = ipv4Address;
	        ss->called.port = (Port);
	    }
	}
    
}

#if 0
/* get media_payload  souceip, source port ipaddrType (v4 or v6) */
void handle_open_receive_channel(skinny_opcode_map_t* skinny_op, u8* msg,u32 len,
                    skinny_info_t* skinny_info)
{
    u8* p = msg;
    u32 conference_id,passthruPartyID,millisecond_packet_size,compression_type;
    u32 qualifer_in[2];
    u32 call_refer;
    char mRxMediaEncryptionKeyInfo[48];
    u32 streamPassThroughID,associatedStreadID,RFC2833PayloadType,dtmfType,mixingMode,partyDirection;
    u32 ipv4or6,sourceIpAddr,sourcePortNumber;
    CW_LOAD_U32(conference_id,p);
    CW_LOAD_U32(passthruPartyID,p);
    CW_LOAD_U32(millisecond_packet_size,p);
    CW_LOAD_U32(compression_type,p);
    if(compression_type == 6) 
        skinny_log(" Media_payload_g722_64k \n");

    
    CW_LOAD_U32(qualifer_in[0],p);
    CW_LOAD_U32(qualifer_in[1],p);
    CW_LOAD_U32(call_refer,p);
    CW_LOAD_STR(mRxMediaEncryptionKeyInfo,p,48);
    
    CW_LOAD_U32(streamPassThroughID,p);
    CW_LOAD_U32(associatedStreadID,p);
    CW_LOAD_U32(RFC2833PayloadType,p);
    CW_LOAD_U32(dtmfType,p);
    
    CW_LOAD_U32(mixingMode,p);
    CW_LOAD_U32(partyDirection,p);
    CW_LOAD_U32(ipv4or6,p);
    CW_LOAD_U32(sourceIpAddr,p);
    CW_LOAD_U32(sourcePortNumber,p);
    
    
    
}
#endif
void handle_default_TimeDate(skinny_opcode_map_t* skinny_op, u8* msg,u32 len,
                    skinny_info_t* skinny_info)
{

	struct tm t;
	int wMilliseconds;
	time_t system_time;
	u8* p = msg;
	
    skinny_log("enter\n");
	struct session_info* ss = skinny_get_session_by_callRef(skinny_info->callid);
	if(ss)
	{
	    memset(&t,0,sizeof(struct tm));
	    CW_LOAD_U32(t.tm_year,p);
	    CW_LOAD_U32(t.tm_mon,p);
	    CW_LOAD_U32(t.tm_wday,p);
	    CW_LOAD_U32(t.tm_mday,p);
	    CW_LOAD_U32(t.tm_hour,p);
	    CW_LOAD_U32(t.tm_min,p);
	    CW_LOAD_U32(t.tm_sec,p);
	    CW_LOAD_U32(wMilliseconds,p);
	    CW_LOAD_U32(wMilliseconds,p);
	    CW_LOAD_U32(system_time,p);
	    skinny_log("time : %d-%d-%d, %d:%d:%d\n",
	    t.tm_year,t.tm_mon,t.tm_mday,
	        t.tm_hour,t.tm_min,t.tm_sec);
	    ss->ring_time.tm_year = t.tm_year - 1900;
	    ss->ring_time.tm_mon = t.tm_mon - 1;
	    ss->ring_time.tm_mday = t.tm_mday;
	    ss->ring_time.tm_wday = t.tm_wday;
	    ss->ring_time.tm_hour = t.tm_hour;
	    ss->ring_time.tm_min = t.tm_min;
	    ss->ring_time.tm_sec = t.tm_sec;
	    
	    
	}
	else
	{
		skinny_log_err("no this callid %s session\n",skinny_info->callid);
	    //exit(0);
	}
}
void handle_CallState(skinny_opcode_map_t* skinny_op, u8* msg,u32 len,
                            skinny_info_t* skinny_info)
{
    
	u8* p = msg;
	u32 callState;
	u32 lineInstance;
	u32 callReference;

	
	struct session_info* ss;
	
	char callid[64]={0};
	
    skinny_log("enter\n");
	CW_LOAD_U32(callState,p);
	CW_LOAD_U32(lineInstance,p);
	CW_LOAD_U32(callReference,p);

	
    skinny_info->callid = callReference;
	sprintf(callid,"%d",callReference);
    ss = si_new_session();
    if(ss ==NULL)
        return NULL;
    ss->call_id = strdup(callid);
    if(ss->call_id == NULL){
        si_del_session(ss);
        return NULL;
    }
	if(ss ==  NULL)
	{
		skinny_log_err("no this callid %s session\n",callid);
		return;
	}
	if(callState == 0x4)
	{
	
	    ss->mode = SS_MODE_CALLED;
	    skinny_log("#########################\n");
	    
	    skinny_log(" I am ringin callstate ,so I am called. \n");
	    
	    skinny_log("#########################\n");
	}
    else if(callState == 0xc)
    {
	    ss->mode = SS_MODE_CALLED;
	    skinny_log("$$$$$$$$$$$$$$$$$$$$$$$$\n");
	    
	    skinny_log(" I am process callstate ,so I am calling. callReference %d \n",callReference);
	    
	    skinny_log("$$$$$$$$$$$$$$$$$$$$$$$$\n");
        
    }
    else
    {
	    skinny_log("^^^^^^^^^^^^^^^^\n");
	    
	    skinny_log(" I callstate %d . \n",callState);
	    
	    skinny_log("^^^^^^^^^\n");
        
    
    }
	
}                            

void handle_DialedNumber(skinny_opcode_map_t* skinny_op, u8* msg,u32 len,
                            skinny_info_t* skinny_info)
{
	u8* p = msg;
	struct session_info* ss;
	char dailed_num[26];
	u32 line_instance=0;
	
	u32 callReference=0;
	char callid[64]={0};
	
    skinny_log("enter\n");

	CW_LOAD_STR(dailed_num,p,25);
	
	CW_LOAD_U32(line_instance,p);
	CW_LOAD_U32(callReference,p);
	sprintf(callid,"%d",callReference);
	
    skinny_info->callid = callReference;
//	ss = skinny_get_session(callid);
	
    ss = si_new_session();
    if(ss ==NULL)
        return;
    ss->call_id = strdup(callid);
    if(ss->call_id == NULL){
        si_del_session(ss);
        return;
    }
	if(ss ==  NULL)
	{
		skinny_log_err("no this callid %s session\n",callid);
		return;
	}
	ss->mode = SS_MODE_CALLING;
	skinny_log("dailed number %s \n",dailed_num);
	
	return;
}



void handle_default_function (skinny_opcode_map_t* skinny_op, u8* msg,u32 len,
    skinny_info_t* skinny_info)
{
	skinny_log("%s and msg len %d  \n",skinny_op->name,len);
	
}

/* Messages Handler Array */
static const skinny_opcode_map_t skinny_opcode_map[] = {
  {0x0000, handle_default_function                                           , SKINNY_MSGTYPE_REQUEST  , "KeepAliveReqMessage"},
#if 1
  {0x0001, handle_default_function                      , SKINNY_MSGTYPE_REQUEST  , "RegisterReqMessage"},
  {0x0002, handle_default_function                           , SKINNY_MSGTYPE_EVENT    , "IpPortMessage"},
  {0x0003, handle_default_function                     , SKINNY_MSGTYPE_EVENT    , "KeypadButtonMessage"},
  {0x0004, handle_default_function                       , SKINNY_MSGTYPE_EVENT    , "EnblocCallMessage"},
  {0x0005, handle_default_function                         , SKINNY_MSGTYPE_EVENT    , "StimulusMessage"},
  {0x0006, handle_default_function                          , SKINNY_MSGTYPE_EVENT    , "OffHookMessage"},
  {0x0007, handle_default_function                           , SKINNY_MSGTYPE_EVENT    , "OnHookMessage"},
  {0x0008, handle_default_function                        , SKINNY_MSGTYPE_EVENT    , "HookFlashMessage"},
  {0x0009, handle_default_function                   , SKINNY_MSGTYPE_REQUEST  , "ForwardStatReqMessage"},
  {0x000a, handle_default_function                 , SKINNY_MSGTYPE_REQUEST  , "SpeedDialStatReqMessage"},
  {0x000b, handle_default_function                      , SKINNY_MSGTYPE_REQUEST  , "LineStatReqMessage"},
  {0x000c, handle_default_function                                           , SKINNY_MSGTYPE_REQUEST  , "ConfigStatReqMessage"},
  {0x000d, handle_default_function                                           , SKINNY_MSGTYPE_REQUEST  , "TimeDateReqMessage"},
  {0x000e, handle_default_function                                           , SKINNY_MSGTYPE_REQUEST  , "ButtonTemplateReqMessage"},
  {0x000f, handle_default_function                                           , SKINNY_MSGTYPE_REQUEST  , "VersionReqMessage"},
  {0x0010, handle_default_function                  , SKINNY_MSGTYPE_RESPONSE , "CapabilitiesResMessage"},
  {0x0012, handle_default_function                                           , SKINNY_MSGTYPE_REQUEST  , "ServerReqMessage"},
  {0x0020, handle_default_function                            , SKINNY_MSGTYPE_EVENT    , "AlarmMessage"},
  {0x0021, handle_default_function       , SKINNY_MSGTYPE_RESPONSE , "MulticastMediaReceptionAckMessage"},
  {0x0022, handle_open_receive_channel_ack            , SKINNY_MSGTYPE_RESPONSE , "OpenReceiveChannelAckMessage"},
  {0x0023, handle_default_function          , SKINNY_MSGTYPE_RESPONSE , "ConnectionStatisticsResMessage"},
  {0x0024, handle_default_function     , SKINNY_MSGTYPE_EVENT    , "OffHookWithCalingPartyNumberMessage"},
  {0x0025, handle_default_function                                           , SKINNY_MSGTYPE_REQUEST  , "SoftKeySetReqMessage"},
  {0x0026, handle_default_function                     , SKINNY_MSGTYPE_EVENT    , "SoftKeyEventMessage"},
  {0x0027, handle_default_function                    , SKINNY_MSGTYPE_REQUEST  , "UnregisterReqMessage"},
  {0x0028, handle_default_function                                           , SKINNY_MSGTYPE_REQUEST  , "SoftKeyTemplateReqMessage"},
  {0x0029, handle_default_function                        , SKINNY_MSGTYPE_REQUEST  , "RegisterTokenReq"},
  {0x002a, handle_default_function         , SKINNY_MSGTYPE_RESPONSE , "MediaTransmissionFailureMessage"},
  {0x002b, handle_default_function                    , SKINNY_MSGTYPE_EVENT    , "HeadsetStatusMessage"},
  {0x002c, handle_default_function        , SKINNY_MSGTYPE_EVENT    , "MediaResourceNotificationMessage"},
  {0x002d, handle_default_function           , SKINNY_MSGTYPE_EVENT    , "RegisterAvailableLinesMessage"},
  {0x002e, handle_default_function                 , SKINNY_MSGTYPE_REQUEST  , "DeviceToUserDataMessage"},
  {0x002f, handle_default_function         , SKINNY_MSGTYPE_RESPONSE , "DeviceToUserDataResponseMessage"},
  {0x0030, handle_default_function               , SKINNY_MSGTYPE_EVENT    , "UpdateCapabilitiesMessage"},
  {0x0031, handle_default_function  , SKINNY_MSGTYPE_RESPONSE , "OpenMultiMediaReceiveChannelAckMessage"},
  {0x0032, handle_default_function                  , SKINNY_MSGTYPE_EVENT    , "ClearConferenceMessage"},
  {0x0033, handle_default_function                , SKINNY_MSGTYPE_REQUEST  , "ServiceURLStatReqMessage"},
  {0x0034, handle_default_function                   , SKINNY_MSGTYPE_REQUEST  , "FeatureStatReqMessage"},
  {0x0035, handle_default_function              , SKINNY_MSGTYPE_RESPONSE , "CreateConferenceResMessage"},
  {0x0036, handle_default_function              , SKINNY_MSGTYPE_RESPONSE , "DeleteConferenceResMessage"},
  {0x0037, handle_default_function              , SKINNY_MSGTYPE_RESPONSE , "ModifyConferenceResMessage"},
  {0x0038, handle_default_function                , SKINNY_MSGTYPE_RESPONSE , "AddParticipantResMessage"},
  {0x0039, handle_default_function               , SKINNY_MSGTYPE_RESPONSE , "AuditConferenceResMessage"},
  {0x0040, handle_default_function              , SKINNY_MSGTYPE_RESPONSE , "AuditParticipantResMessage"},
  {0x0041, handle_default_function         , SKINNY_MSGTYPE_REQUEST  , "DeviceToUserDataMessageVersion1"},
  {0x0042, handle_default_function , SKINNY_MSGTYPE_RESPONSE , "DeviceToUserDataResponseMessageVersion1"},
  {0x0043, handle_default_function                , SKINNY_MSGTYPE_RESPONSE , "CapabilitiesV2ResMessage"},
  {0x0044, handle_default_function                , SKINNY_MSGTYPE_RESPONSE , "CapabilitiesV3ResMessage"},
  {0x0045, handle_default_function                          , SKINNY_MSGTYPE_RESPONSE , "PortResMessage"},
  {0x0046, handle_default_function                    , SKINNY_MSGTYPE_EVENT    , "QoSResvNotifyMessage"},
  {0x0047, handle_default_function                   , SKINNY_MSGTYPE_EVENT    , "QoSErrorNotifyMessage"},
  {0x0048, handle_default_function              , SKINNY_MSGTYPE_REQUEST  , "SubscriptionStatReqMessage"},
  {0x0049, handle_default_function                   , SKINNY_MSGTYPE_EVENT    , "MediaPathEventMessage"},
  {0x004a, handle_default_function              , SKINNY_MSGTYPE_EVENT    , "MediaPathCapabilityMessage"},
  {0x004c, handle_default_function                  , SKINNY_MSGTYPE_REQUEST  , "MwiNotificationMessage"},
  {0x0081, handle_default_function                      , SKINNY_MSGTYPE_RESPONSE , "RegisterAckMessage"},
  {0x0082, handle_default_function                        , SKINNY_MSGTYPE_EVENT    , "StartToneMessage"},
  {0x0083, handle_default_function                         , SKINNY_MSGTYPE_EVENT    , "StopToneMessage"},
  {0x0085, handle_default_function                        , SKINNY_MSGTYPE_EVENT    , "SetRingerMessage"},
  {0x0086, handle_default_function                          , SKINNY_MSGTYPE_EVENT    , "SetLampMessage"},
  {0x0087, handle_default_function                                           , SKINNY_MSGTYPE_EVENT    , "SetHookFlashDetectMessage"},
  {0x0088, handle_default_function                   , SKINNY_MSGTYPE_EVENT    , "SetSpeakerModeMessage"},
  {0x0089, handle_default_function                     , SKINNY_MSGTYPE_EVENT    , "SetMicroModeMessage"},
  {0x008a, handle_start_media_transmission           , SKINNY_MSGTYPE_REQUEST  , "StartMediaTransmissionMessage"},
  {0x008b, handle_default_function   , SKINNY_MSGTYPE_EVENT    , "StopMediaTransmissionMessage"},
  {0x008f, handle_default_function                         , SKINNY_MSGTYPE_EVENT    , "CallInfoMessage"},
  {0x0090, handle_default_function                   , SKINNY_MSGTYPE_RESPONSE , "ForwardStatResMessage"},
  {0x0091, handle_default_function                 , SKINNY_MSGTYPE_RESPONSE , "SpeedDialStatResMessage"},
  {0x0092, handle_default_function                      , SKINNY_MSGTYPE_RESPONSE , "LineStatResMessage"},
  {0x0093, handle_default_function                    , SKINNY_MSGTYPE_RESPONSE , "ConfigStatResMessage"},
  //ä»ŽTimeDateResMessageé‡Œå–æ—¶é—´ã€‚
  {0x0094, handle_default_TimeDate                      , SKINNY_MSGTYPE_RESPONSE , "TimeDateResMessage"},//---liudan
  {0x0095, handle_default_function         , SKINNY_MSGTYPE_EVENT    , "StartSessionTransmissionMessage"},
  {0x0096, handle_default_function          , SKINNY_MSGTYPE_EVENT    , "StopSessionTransmissionMessage"},
  {0x0097, handle_default_function                , SKINNY_MSGTYPE_RESPONSE , "ButtonTemplateResMessage"},
  {0x0098, handle_default_function                       , SKINNY_MSGTYPE_RESPONSE , "VersionResMessage"},
  {0x0099, handle_default_function                      , SKINNY_MSGTYPE_EVENT    , "DisplayTextMessage"},
  {0x009a, handle_default_function                                           , SKINNY_MSGTYPE_EVENT    , "ClearDisplay"},
  {0x009b, handle_default_function                                           , SKINNY_MSGTYPE_EVENT    , "CapabilitiesReq"},
  {0x009d, handle_default_function                   , SKINNY_MSGTYPE_EVENT    , "RegisterRejectMessage"},
  {0x009e, handle_default_function                        , SKINNY_MSGTYPE_RESPONSE , "ServerResMessage"},
  {0x009f, handle_default_function                                   , SKINNY_MSGTYPE_EVENT    , "Reset"},
  {0x0100, handle_default_function                                           , SKINNY_MSGTYPE_RESPONSE , "KeepAliveAckMessage"},
  {0x0101, handle_default_function     , SKINNY_MSGTYPE_REQUEST  , "StartMulticastMediaReceptionMessage"},
  {0x0102, handle_default_function  , SKINNY_MSGTYPE_REQUEST  , "StartMulticastMediaTransmissionMessage"},
  {0x0103, handle_default_function      , SKINNY_MSGTYPE_EVENT    , "StopMulticastMediaReceptionMessage"},
  {0x0104, handle_default_function   , SKINNY_MSGTYPE_EVENT    , "StopMulticastMediaTransmissionMessage"},
  {0x0105, handle_default_function               , SKINNY_MSGTYPE_REQUEST  , "OpenReceiveChannelMessage"},
  {0x0106, handle_default_function              , SKINNY_MSGTYPE_EVENT    , "CloseReceiveChannelMessage"},
  {0x0107, handle_default_function          , SKINNY_MSGTYPE_REQUEST  , "ConnectionStatisticsReqMessage"},
  {0x0108, handle_default_function               , SKINNY_MSGTYPE_RESPONSE , "SoftKeyTemplateResMessage"},
  {0x0109, handle_default_function                    , SKINNY_MSGTYPE_RESPONSE , "SoftKeySetResMessage"},
  {0x0110, handle_default_function                   , SKINNY_MSGTYPE_EVENT    , "SelectSoftKeysMessage"},
  {0x0111, handle_CallState                        , SKINNY_MSGTYPE_EVENT    , "CallStateMessage"},
  {0x0112, handle_default_function              , SKINNY_MSGTYPE_EVENT    , "DisplayPromptStatusMessage"},
  {0x0113, handle_clear_prompt_status                , SKINNY_MSGTYPE_EVENT    ,"ClearPromptStatusMessage"},
  {0x0114, handle_default_function                    , SKINNY_MSGTYPE_EVENT    , "DisplayNotifyMessage"},
  {0x0115, handle_default_function                                           , SKINNY_MSGTYPE_EVENT    , "ClearNotifyMessage"},
  {0x0116, handle_default_function                , SKINNY_MSGTYPE_EVENT    , "ActivateCallPlaneMessage"},
  {0x0117, handle_default_function                                           , SKINNY_MSGTYPE_EVENT    , "DeactivateCallPlaneMessage"},
  {0x0118, handle_default_function                    , SKINNY_MSGTYPE_RESPONSE , "UnregisterAckMessage"},
  {0x0119, handle_default_function                     , SKINNY_MSGTYPE_EVENT    , "BackSpaceResMessage"},
  {0x011a, handle_default_function                                           , SKINNY_MSGTYPE_RESPONSE , "RegisterTokenAck"},
  {0x011b, handle_default_function                     , SKINNY_MSGTYPE_RESPONSE , "RegisterTokenReject"},
  {0x011c, handle_default_function       , SKINNY_MSGTYPE_EVENT    , "StartMediaFailureDetectionMessage"},
  {0x011d, handle_DialedNumber                     , SKINNY_MSGTYPE_EVENT    , "DialedNumberMessage"}, //æåˆ°ç”¨æˆ·è¾“å…¥çš„å·ç ã€‚
  {0x011e, handle_default_function                 , SKINNY_MSGTYPE_EVENT    , "UserToDeviceDataMessage"},
  {0x011f, handle_default_function                   , SKINNY_MSGTYPE_RESPONSE , "FeatureStatResMessage"},
  {0x0120, handle_default_function                 , SKINNY_MSGTYPE_EVENT    , "DisplayPriNotifyMessage"},
  {0x0121, handle_default_function                   , SKINNY_MSGTYPE_EVENT    , "ClearPriNotifyMessage"},
  {0x0122, handle_default_function                , SKINNY_MSGTYPE_EVENT    , "StartAnnouncementMessage"},
  {0x0123, handle_default_function                 , SKINNY_MSGTYPE_EVENT    , "StopAnnouncementMessage"},
  {0x0124, handle_default_function               , SKINNY_MSGTYPE_EVENT    , "AnnouncementFinishMessage"},
  {0x0127, handle_default_function                   , SKINNY_MSGTYPE_EVENT    , "NotifyDtmfToneMessage"},
  {0x0128, handle_default_function                     , SKINNY_MSGTYPE_EVENT    , "SendDtmfToneMessage"},
  {0x0129, handle_default_function          , SKINNY_MSGTYPE_REQUEST  , "SubscribeDtmfPayloadReqMessage"},
  {0x012a, handle_default_function          , SKINNY_MSGTYPE_RESPONSE , "SubscribeDtmfPayloadResMessage"},
  {0x012b, handle_default_function          , SKINNY_MSGTYPE_RESPONSE , "SubscribeDtmfPayloadErrMessage"},
  {0x012c, handle_default_function        , SKINNY_MSGTYPE_REQUEST  , "UnSubscribeDtmfPayloadReqMessage"},
  {0x012d, handle_default_function        , SKINNY_MSGTYPE_RESPONSE , "UnSubscribeDtmfPayloadResMessage"},
  {0x012e, handle_default_function        , SKINNY_MSGTYPE_RESPONSE , "UnSubscribeDtmfPayloadErrMessage"},
  {0x012f, handle_default_function                , SKINNY_MSGTYPE_RESPONSE , "ServiceURLStatResMessage"},
  {0x0130, handle_default_function                , SKINNY_MSGTYPE_EVENT    , "CallSelectStatResMessage"},
  {0x0131, handle_default_function     , SKINNY_MSGTYPE_REQUEST  , "OpenMultiMediaReceiveChannelMessage"},
  {0x0132, handle_default_function      , SKINNY_MSGTYPE_REQUEST  , "StartMultiMediaTransmissionMessage"},
  {0x0133, handle_default_function       , SKINNY_MSGTYPE_EVENT    , "StopMultiMediaTransmissionMessage"},
  {0x0134, handle_default_function             , SKINNY_MSGTYPE_EVENT    , "MiscellaneousCommandMessage"},
  {0x0135, handle_default_function               , SKINNY_MSGTYPE_EVENT    , "FlowControlCommandMessage"},
  {0x0136, handle_default_function    , SKINNY_MSGTYPE_EVENT    , "CloseMultiMediaReceiveChannelMessage"},
  {0x0137, handle_default_function              , SKINNY_MSGTYPE_REQUEST  , "CreateConferenceReqMessage"},
  {0x0138, handle_default_function              , SKINNY_MSGTYPE_REQUEST  , "DeleteConferenceReqMessage"},
  {0x0139, handle_default_function              , SKINNY_MSGTYPE_REQUEST  , "ModifyConferenceReqMessage"},
  {0x013a, handle_default_function                , SKINNY_MSGTYPE_REQUEST  , "AddParticipantReqMessage"},
  {0x013b, handle_default_function               , SKINNY_MSGTYPE_REQUEST  , "DropParticipantReqMessage"},
  {0x013c, handle_default_function                                           , SKINNY_MSGTYPE_REQUEST  , "AuditConferenceReqMessage"},
  {0x013d, handle_default_function              , SKINNY_MSGTYPE_REQUEST  , "AuditParticipantReqMessage"},
  {0x013e, handle_default_function             , SKINNY_MSGTYPE_REQUEST  , "ChangeParticipantReqMessage"},
  {0x013f, handle_default_function         , SKINNY_MSGTYPE_EVENT    , "UserToDeviceDataMessageVersion1"},
  {0x0140, handle_default_function              , SKINNY_MSGTYPE_EVENT    , "VideoDisplayCommandMessage"},
  {0x0141, handle_default_function                , SKINNY_MSGTYPE_EVENT    , "FlowControlNotifyMessage"},
  {0x0142, handle_default_function                  , SKINNY_MSGTYPE_RESPONSE , "ConfigStatV2ResMessage"},
  {0x0143, handle_default_function                  , SKINNY_MSGTYPE_EVENT    , "DisplayNotifyV2Message"},
  {0x0144, handle_default_function               , SKINNY_MSGTYPE_EVENT    , "DisplayPriNotifyV2Message"},
  {0x0145, handle_default_function            , SKINNY_MSGTYPE_EVENT    , "DisplayPromptStatusV2Message"},
  {0x0146, handle_default_function                 , SKINNY_MSGTYPE_RESPONSE , "FeatureStatV2ResMessage"},
  {0x0147, handle_default_function                    , SKINNY_MSGTYPE_RESPONSE , "LineStatV2ResMessage"},
  {0x0148, handle_default_function              , SKINNY_MSGTYPE_RESPONSE , "ServiceURLStatV2ResMessage"},
  {0x0149, handle_default_function               , SKINNY_MSGTYPE_RESPONSE , "SpeedDialStatV2ResMessage"},
  {0x014a, handle_default_function                       , SKINNY_MSGTYPE_EVENT    , "CallInfoV2Message"},
  {0x014b, handle_default_function                          , SKINNY_MSGTYPE_REQUEST  , "PortReqMessage"},
  {0x014c, handle_default_function                        , SKINNY_MSGTYPE_EVENT    , "PortCloseMessage"},
  {0x014d, handle_default_function                        , SKINNY_MSGTYPE_EVENT    , "QoSListenMessage"},
  {0x014e, handle_default_function                          , SKINNY_MSGTYPE_EVENT    , "QoSPathMessage"},
  {0x014f, handle_default_function                      , SKINNY_MSGTYPE_EVENT    , "QoSTeardownMessage"},
  {0x0150, handle_default_function                       , SKINNY_MSGTYPE_EVENT    , "UpdateDSCPMessage"},
  {0x0151, handle_default_function                        , SKINNY_MSGTYPE_EVENT    , "QoSModifyMessage"},
  {0x0152, handle_default_function              , SKINNY_MSGTYPE_RESPONSE , "SubscriptionStatResMessage"},
  {0x0153, handle_default_function                     , SKINNY_MSGTYPE_EVENT    , "NotificationMessage"},
  {0x0154, handle_default_function        , SKINNY_MSGTYPE_RESPONSE , "StartMediaTransmissionAckMessage"},
  {0x0155, handle_default_function   , SKINNY_MSGTYPE_RESPONSE , "StartMultiMediaTransmissionAckMessage"},
  {0x0156, handle_default_function                  , SKINNY_MSGTYPE_EVENT    , "CallHistoryInfoMessage"},
  {0x0157, handle_default_function                     , SKINNY_MSGTYPE_EVENT    , "LocationInfoMessage"},
  {0x0158, handle_default_function                           , SKINNY_MSGTYPE_RESPONSE , "MwiResMessage"},
  {0x0159, handle_default_function          , SKINNY_MSGTYPE_EVENT    , "AddOnDeviceCapabilitiesMessage"},
  {0x015a, handle_default_function                    , SKINNY_MSGTYPE_EVENT    , "EnhancedAlarmMessage"},
  {0x015e, handle_default_function                                           , SKINNY_MSGTYPE_REQUEST  , "CallCountReqMessage"},
  {0x015f, handle_default_function                    , SKINNY_MSGTYPE_RESPONSE , "CallCountRespMessage"},
  {0x0160, handle_default_function                  , SKINNY_MSGTYPE_EVENT    , "RecordingStatusMessage"},
  {0x8000, handle_default_function                    , SKINNY_MSGTYPE_REQUEST  , "SPCPRegisterTokenReq"},
  {0x8100, handle_default_function                    , SKINNY_MSGTYPE_RESPONSE , "SPCPRegisterTokenAck"},
  {0x8101, handle_default_function                 , SKINNY_MSGTYPE_RESPONSE , "SPCPRegisterTokenReject"},
#endif
};

#define CW_IsMsgOutBound(pucCurPos, pucEndPos, usLen) ((pucCurPos + usLen) > pucEndPos)

int handler_skinny_elements(u8* msg,int msg_size)
{
	u32 len;
	u32 hdr_opcode;
	u32 hdr_ver;
	u8* p = msg;
	int i;
	u32 message_len = 0;
	skinny_opcode_map_t* opcode_entry;

	skinny_info_t skinny_info;
	memset(&skinny_info,0,sizeof(skinny_info_t));
	
	while(p+12 < msg+msg_size)
	{
		CW_LOAD_U32(len,p);
		CW_LOAD_U32(hdr_ver,p);

		CW_LOAD_U32(hdr_opcode,p);
		message_len = len -4;
		skinny_log("len %x (%x) ver %x opcode %x\n",len,htonl(len),hdr_ver,hdr_opcode);
		if(p+message_len <= msg+msg_size)
		{
			for (i = 0; i < sizeof(skinny_opcode_map)/sizeof(skinny_opcode_map_t) ; i++) 
			{
	    		if (skinny_opcode_map[i].opcode == hdr_opcode) 
				{
	     			 opcode_entry = &skinny_opcode_map[i];
					 opcode_entry->handler(opcode_entry,p,len,&skinny_info);
					 p+=message_len;
					 break;
	    		}
  			}
			
		}
		else
			skinny_log_err("opcode %d msg broken!\n",hdr_opcode);
			
	}
}
void show_tcp_info(struct tcphdr* th )
{
	
	skinny_log("tcp info :\n"
				"source port %u, dest port %u\n "
				"Sequence Number %u,Acknowledgement Number %u\n "
				"head len %d byte (%d)\n"
				"flags %s %s %s %s %s %s %s %s \n",
			htons(th->source),htons(th->dest),ntohl(th->seq),          ntohl(th->ack_seq),
				th->doff*4,th->doff,
				th->fin?"Finish":"NoFinish",
				th->syn?"Syn":"NoSyn",
				th->rst?"Reset":"NoReset",
				th->psh?"Push":"NoPush",
				th->ack?"Ack":"NoAck",
				th->urg?"Urgent":"NoUrgent",
				th->ece?"ECN-ECHO":"NoECH-ECHO",
				th->cwr?"Congestion_Window_Reduced":"NoCWR");
}
dump_hex(u8* src,int len)
{
    u8 buffer[8000]={0};
    int i;
    int t;
    u8* p = buffer;
    for(i=0;i<len;i++){
        t = sprintf(p,"%02X ",src[i]);
        p+=t;
    }
    skinny_log("%s\n",buffer);
}
static void sniffer_handle_skinny(u_char * user, const struct pcap_pkthdr * packet_header, const u_char * packet_content)
{
    int ret = 0;
    
	const struct pcap_pkthdr* phdr = packet_header;
	struct iphdr* iph = NULL;
	struct tcphdr* th = NULL;
	u8* tcp_payload;
	int tcp_payload_len;
	int tcp_len;
	
	
	skinny_log("\n\n");
	ret = check_iphdr(phdr,packet_content,&iph);
	if(ret != 0){
		
		skinny_log_err("check_iphdr error\n");
		goto error;
    }
	if(0 != check_tcp(iph,&th))	{
		
		skinny_log_err("check_tcp error\n");
		goto error;
	}
	
    //send_sip_pkt(iph,udph);/* æŠŠsipæŠ¥æ–‡è½¬ç»™uploadä¸€ä»½ã€‚ */

	tcp_payload = ((u8*)th)+(th->doff * 4);
	tcp_len = ntohs(iph->tot_len)- iph->ihl*4;
	tcp_payload_len = tcp_len - (th->doff * 4);
	show_tcp_info(th);
	if(tcp_payload_len == 0)
	{
		skinny_log("this frame no tcp payload\n\n");
		return;
	}
	
	skinny_log("tcp_payload %p, tcp_payload_len  %d th %p \n",tcp_payload,
	    tcp_payload_len,th);
	dump_hex(tcp_payload,tcp_payload_len);
	handler_skinny_elements(tcp_payload,tcp_payload_len);
	
error:
	return;
}

int sniffer_loop_skinny( pcap_t *p)
{
	 pcap_loop( p,-1,sniffer_handle_skinny,(u_char*)NULL);
	 return 0;
}

/***********************************************
çº¿ç¨‹å¯åŠ¨ä¸Žæ‰§è¡Œä½“
************************************************/
/*
sniffer_sip_loop:
æ‰“å¼€pcap_file,è¿›å…¥æŠ“åŒ…æ‰§è¡Œä½“ã€‚

*/
void* sniffer_skinny_loop(void* arg)
{

	char filter[200] = {0};
    pcap_t* pd=0;

	//pd = open_pcap_file("enp0s3",65535,1,0);
	pd = open_pcap_file("eth0",65535,1,0);
	if(pd == NULL)
	{

		skinny_log_err("open_pcap_file failed ! <%s> and exit\n",strerror(errno));
		exit(1);
	}

	sprintf(filter,"tcp and host %s and port %d ",
		inet_ntoa(g_config.skinny_call.ip),
		g_config.skinny_call.port);
	sniffer_setfilter(pd,filter);
	
    skinny_log("sniffer_skinny_loop ok  \n");

	while(1)
	{
		sniffer_loop_skinny(pd);
	}
}


pthread_t __sniffer_skinny_start(void)
{
	pthread_t tid;
	if(pthread_create(&tid,NULL,sniffer_skinny_loop,NULL))
	{
		skinny_log_err("create  sniffer_skinny_loop failed\n");
		return -1;
	}

	return tid;

}



pthread_t sniffer_skinny_start(void)
{
	pthread_t tid;

	tid = __sniffer_skinny_start();
    skinny_log("%s:%d tid %d\n",__func__,__LINE__,tid);
	return tid;

}

