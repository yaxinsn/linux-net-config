




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
  { 0x0094, "TimeDateRes" },---------liudan
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
static value_string_ext message_id_ext = VALUE_STRING_EXT_INIT(message_id);

/* Declare Enums and Defines */
static const value_string DisplayLabels_36[] = {
  { 0x00000, "Empty" },
  { 0x00002, "Acct" },
  { 0x00003, "Flash" },
  { 0x00004, "Login" },
  { 0x00005, "Device In Home Location" },
  { 0x00006, "Device In Roaming Location" },
  { 0x00007, "Enter Authorization Code" },
  { 0x00008, "Enter Client Matter Code" },
  { 0x00009, "Calls Available For Pickup" },
  { 0x0000a, "Cm Fallback Service Operating" },
  { 0x0000b, "Max Phones Exceeded" },
  { 0x0000c, "Waiting To Rehome" },
  { 0x0000d, "Please End Call" },
  { 0x0000e, "Paging" },
  { 0x0000f, "Select Line" },
  { 0x00010, "Transfer Destination Is Busy" },
  { 0x00011, "Select A Service" },
  { 0x00012, "Local Services" },
  { 0x00013, "Enter Search Criteria" },
  { 0x00014, "Night Service" },
  { 0x00015, "Night Service Active" },
  { 0x00016, "Night Service Disabled" },
  { 0x00017, "Login Successful" },
  { 0x00018, "Wrong Pin" },
  { 0x00019, "Please Enter Pin" },
  { 0x0001a, "Of" },
  { 0x0001b, "Records 1 To" },
  { 0x0001c, "No Record Found" },
  { 0x0001d, "Search Results" },
  { 0x0001e, "Calls In Queue" },
  { 0x0001f, "Join To Hunt Group" },
  { 0x00020, "Ready" },
  { 0x00021, "Notready" },
  { 0x00022, "Call On Hold" },
  { 0x00023, "Hold Reversion" },
  { 0x00024, "Setup Failed" },
  { 0x00025, "No Resources" },
  { 0x00026, "Device Not Authorized" },
  { 0x00027, "Monitoring" },
  { 0x00028, "Recording Awaiting Call To Be Active" },
  { 0x00029, "Recording Already In Progress" },
  { 0x0002a, "Inactive Recording Session" },
  { 0x0002b, "Mobility" },
  { 0x0002c, "Whisper" },
  { 0x0002d, "Forward All" },
  { 0x0002e, "Malicious Call Id" },
  { 0x0002f, "Group Pickup" },
  { 0x00030, "Remove Last Participant" },
  { 0x00031, "Other Pickup" },
  { 0x00032, "Video" },
  { 0x00033, "End Call" },
  { 0x00034, "Conference List" },
  { 0x00035, "Quality Reporting Tool" },
  { 0x00036, "Hunt Group" },
  { 0x00037, "Use Line Or Join To Complete" },
  { 0x00038, "Do Not Disturb" },
  { 0x00039, "Do Not Disturb Is Active" },
  { 0x0003a, "Cfwdall Loop Detected" },
  { 0x0003b, "Cfwdall Hops Exceeded" },
  { 0x0003c, "Abbrdial" },
  { 0x0003d, "Pickup Is Unavailable" },
  { 0x0003e, "Conference Is Unavailable" },
  { 0x0003f, "Meetme Is Unavailable" },
  { 0x00040, "Cannot Retrieve Parked Call" },
  { 0x00041, "Cannot Send Call To Mobile" },
  { 0x00043, "Record" },
  { 0x00044, "Cannot Move Conversation" },
  { 0x00045, "Cw Off" },
  { 0x00046, "Coaching" },
  { 0x0004f, "Recording" },
  { 0x00050, "Recording Failed" },
  { 0x00051, "Connecting" },
  { 0x00000, NULL }
};
static value_string_ext DisplayLabels_36_ext = VALUE_STRING_EXT_INIT(DisplayLabels_36);

static const value_string DisplayLabels_200[] = {
  { 0x00001, "Redial" },
  { 0x00002, "Newcall" },
  { 0x00003, "Hold" },
  { 0x00004, "Transfer" },
  { 0x00005, "Cfwdall" },
  { 0x00006, "Cfwdbusy" },
  { 0x00007, "Cfwdnoanswer" },
  { 0x00008, "Backspace" },
  { 0x00009, "Endcall" },
  { 0x0000a, "Resume" },
  { 0x0000b, "Answer" },
  { 0x0000c, "Info" },
  { 0x0000d, "Confrn" },
  { 0x0000e, "Park" },
  { 0x0000f, "Join" },
  { 0x00010, "Meetme" },
  { 0x00011, "Pickup" },
  { 0x00012, "Gpickup" },
  { 0x00013, "Your Current Options" },
  { 0x00014, "Off Hook" },
  { 0x00015, "On Hook" },
  { 0x00016, "Ring Out" },
  { 0x00017, "From" },
  { 0x00018, "Connected" },
  { 0x00019, "Busy" },
  { 0x0001a, "Line In Use" },
  { 0x0001b, "Call Waiting" },
  { 0x0001c, "Call Transfer" },
  { 0x0001d, "Call Park" },
  { 0x0001e, "Call Proceed" },
  { 0x0001f, "In Use Remote" },
  { 0x00020, "Enter Number" },
  { 0x00021, "Call Park At" },
  { 0x00022, "Primary Only" },
  { 0x00023, "Temp Fail" },
  { 0x00024, "You Have Voicemail" },
  { 0x00025, "Forwarded To" },
  { 0x00026, "Can Not Complete Conference" },
  { 0x00027, "No Conference Bridge" },
  { 0x00028, "Can Not Hold Primary Control" },
  { 0x00029, "Invalid Conference Participant" },
  { 0x0002a, "In Conference Already" },
  { 0x0002b, "No Participant Info" },
  { 0x0002c, "Exceed Maximum Parties" },
  { 0x0002d, "Key Is Not Active" },
  { 0x0002e, "Error No License" },
  { 0x0002f, "Error Dbconfig" },
  { 0x00030, "Error Database" },
  { 0x00031, "Error Pass Limit" },
  { 0x00032, "Error Unknown" },
  { 0x00033, "Error Mismatch" },
  { 0x00034, "Conference" },
  { 0x00035, "Park Number" },
  { 0x00036, "Private" },
  { 0x00037, "Not Enough Bandwidth" },
  { 0x00038, "Unknown Number" },
  { 0x00039, "Rmlstc" },
  { 0x0003a, "Voicemail" },
  { 0x0003b, "Immdiv" },
  { 0x0003c, "Intrcpt" },
  { 0x0003d, "Setwtch" },
  { 0x0003e, "Trnsfvm" },
  { 0x0003f, "Dnd" },
  { 0x00040, "Divall" },
  { 0x00041, "Callback" },
  { 0x00042, "Network Congestion Rerouting" },
  { 0x00043, "Barge" },
  { 0x00044, "Failed To Setup Barge" },
  { 0x00045, "Another Barge Exists" },
  { 0x00046, "Incompatible Device Type" },
  { 0x00047, "No Park Number Available" },
  { 0x00048, "Callpark Reversion" },
  { 0x00049, "Service Is Not Active" },
  { 0x0004a, "High Traffic Try Again Later" },
  { 0x0004b, "Qrt" },
  { 0x0004c, "Mcid" },
  { 0x0004d, "Dirtrfr" },
  { 0x0004e, "Select" },
  { 0x0004f, "Conflist" },
  { 0x00050, "Idivert" },
  { 0x00051, "Cbarge" },
  { 0x00052, "Can Not Complete Transfer" },
  { 0x00053, "Can Not Join Calls" },
  { 0x00054, "Mcid Successful" },
  { 0x00055, "Number Not Configured" },
  { 0x00056, "Security Error" },
  { 0x00057, "Video Bandwidth Unavailable" },
  { 0x00058, "Vidmode" },
  { 0x00059, "Max Call Duration Timeout" },
  { 0x0005a, "Max Hold Duration Timeout" },
  { 0x0005b, "Opickup" },
  { 0x0005c, "Hlog" },
  { 0x0005d, "Logged Out Of Hunt Group" },
  { 0x0005e, "Park Slot Unavailable" },
  { 0x0005f, "No Call Available For Pickup" },
  { 0x00061, "External Transfer Restricted" },
  { 0x00062, "No Line Available For Pickup" },
  { 0x00063, "Path Replacement In Progress" },
  { 0x00064, "Unknown 2" },
  { 0x00065, "Mac Address" },
  { 0x00066, "Host Name" },
  { 0x00067, "Domain Name" },
  { 0x00068, "Ip Address" },
  { 0x00069, "Subnet Mask" },
  { 0x0006a, "Tftp Server 1" },
  { 0x0006b, "Default Router 1" },
  { 0x0006c, "Default Router 2" },
  { 0x0006d, "Default Router 3" },
  { 0x0006e, "Default Router 4" },
  { 0x0006f, "Default Router 5" },
  { 0x00070, "Dns Server 1" },
  { 0x00071, "Dns Server 2" },
  { 0x00072, "Dns Server 3" },
  { 0x00073, "Dns Server 4" },
  { 0x00074, "Dns Server 5" },
  { 0x00075, "Operational Vlan Id" },
  { 0x00076, "Admin Vlan Id" },
  { 0x00077, "Call Manager 1" },
  { 0x00078, "Call Manager 2" },
  { 0x00079, "Call Manager 3" },
  { 0x0007a, "Call Manager 4" },
  { 0x0007b, "Call Manager 5" },
  { 0x0007c, "Information Url" },
  { 0x0007d, "Directories Url" },
  { 0x0007e, "Messages Url" },
  { 0x0007f, "Services Url" },
  { 0x00000, NULL }
};
static value_string_ext DisplayLabels_200_ext = VALUE_STRING_EXT_INIT(DisplayLabels_200);

static const value_string DeviceType[] = {
  { 0x00001, "Station30SPplus" },
  { 0x00002, "Station12SPplus" },
  { 0x00003, "Station12SP" },
  { 0x00004, "Station12" },
  { 0x00005, "Station30VIP" },
  { 0x00006, "Cisco 7910" },
  { 0x00007, "StationTelecasterMgr" },
  { 0x00008, "Cisco 7940" },
  { 0x00009, "Cisco 7935" },
  { 0x0000a, "StationVGC" },
  { 0x0000b, "VGCVirtualPhone" },
  { 0x0000c, "StationATA186" },
  { 0x0000d, "StationATA188" },
  { 0x0000f, "EmccBase" },
  { 0x00014, "Virtual30SPplus" },
  { 0x00015, "StationPhoneApplication" },
  { 0x0001e, "AnalogAccess" },
  { 0x00028, "DigitalAccessTitan1" },
  { 0x00029, "Digital Access T1" },
  { 0x0002a, "DigitalAccessTitan2" },
  { 0x0002b, "DigitalAccessLennon" },
  { 0x0002f, "AnalogAccessElvis" },
  { 0x00030, "VGCGateway" },
  { 0x00032, "ConferenceBridge" },
  { 0x00033, "ConferenceBridgeYoko" },
  { 0x00034, "ConferenceBridgeDixieLand" },
  { 0x00035, "ConferenceBridgeSummit" },
  { 0x0003c, "H225" },
  { 0x0003d, "H323Phone" },
  { 0x0003e, "H323Gateway" },
  { 0x00046, "MusicOnHold" },
  { 0x00047, "Pilot" },
  { 0x00048, "TapiPort" },
  { 0x00049, "TapiRoutePoint" },
  { 0x00050, "VoiceInBox" },
  { 0x00051, "VoiceInboxAdmin" },
  { 0x00052, "LineAnnunciator" },
  { 0x00053, "SoftwareMtpDixieLand" },
  { 0x00054, "CiscoMediaServer" },
  { 0x00055, "ConferenceBridgeFlint" },
  { 0x00056, "ConferenceBridgeHetroGen" },
  { 0x00057, "ConferenceBridgeAudVid" },
  { 0x00058, "ConferenceHVideoBridge" },
  { 0x0005a, "RouteList" },
  { 0x00064, "LoadSimulator" },
  { 0x0006e, "MediaTerminationPoint" },
  { 0x0006f, "MediaTerminationPointYoko" },
  { 0x00070, "MediaTerminationPointDixieLand" },
  { 0x00071, "MediaTerminationPointSummit" },
  { 0x00073, "7941G" },
  { 0x00077, "7971" },
  { 0x00078, "MGCPStation" },
  { 0x00079, "MGCPTrunk" },
  { 0x0007a, "RASProxy" },
  { 0x0007c, "Cisco 7914 AddOn" },
  { 0x0007d, "Trunk" },
  { 0x0007e, "Annunciator" },
  { 0x0007f, "MonitorBridge" },
  { 0x00080, "Recorder" },
  { 0x00081, "MonitorBridgeYoko" },
  { 0x00083, "SipTrunk" },
  { 0x00084, "SipGateway" },
  { 0x00085, "WsmTrunk" },
  { 0x00086, "RemoteDestination" },
  { 0x000e3, "Cisco 7915 AddOn" },
  { 0x000e4, "Cisco 7915 AddOn 24" },
  { 0x000e5, "Cisco 7916 AddOn" },
  { 0x000e6, "Cisco 7916 AddOn 24" },
  { 0x000fd, "GenericDevice" },
  { 0x000fe, "UnknownMGCPGateway" },
  { 0x000ff, "NotDefined" },
  { 0x00113, "Nokia E Series" },
  { 0x0012e, "Cisco 7985" },
  { 0x00133, "7911" },
  { 0x00134, "Cisco 7961 GE" },
  { 0x00135, "7961G_GE" },
  { 0x0014f, "MotorolaCN622" },
  { 0x00150, "3rdPartySipBasic" },
  { 0x0015c, "Cisco 7931" },
  { 0x00166, "UnifiedCommunicator" },
  { 0x0016d, "7921" },
  { 0x00171, "7906" },
  { 0x00176, "3rdPartySipAdv" },
  { 0x00177, "Telepresence" },
  { 0x00178, "Nokia ICC client" },
  { 0x00194, "7962" },
  { 0x0019c, "3951" },
  { 0x001af, "7937" },
  { 0x001b2, "7942" },
  { 0x001b3, "7945" },
  { 0x001b4, "7965" },
  { 0x001b5, "7975" },
  { 0x001d4, "UnifiedMobileCommunicator" },
  { 0x001e4, "Cisco 7925" },
  { 0x001ed, "9971_CE" },
  { 0x001ef, "Cisco 6921" },
  { 0x001f0, "Cisco 6941" },
  { 0x001f1, "Cisco 6961" },
  { 0x001f7, "CSF" },
  { 0x00223, "Cisco 6901" },
  { 0x00224, "Cisco 6911" },
  { 0x00234, "Cisco 6945" },
  { 0x00249, "Cisco 8945" },
  { 0x0024a, "Cisco 8941" },
  { 0x00255, "CiscoTelepresenceMcu" },
  { 0x00257, "CiscoTelePresenceExchange" },
  { 0x00258, "CiscoTelePresenceSoftwareConferenceBridge" },
  { 0x00277, "ASSip" },
  { 0x0027b, "CtiRemoteDevice" },
  { 0x04e20, "7905" },
  { 0x07532, "7920" },
  { 0x07536, "7970" },
  { 0x07537, "7912" },
  { 0x07538, "7902" },
  { 0x07540, "Cisco IP Communicator" },
  { 0x07542, "7961G" },
  { 0x07543, "7936" },
  { 0x0754b, "AnalogPhone" },
  { 0x0754c, "ISDNBRIPhone" },
  { 0x07550, "SCCPGwVirtualPhone" },
  { 0x07553, "IP_STE" },
  { 0x08cc9, "CiscoTelePresenceConductor" },
  { 0x08d7b, "InteractiveVoiceResponse" },
  { 0x13880, "Cisco SPA 521S" },
  { 0x13883, "Cisco SPA 502G" },
  { 0x13884, "Cisco SPA 504G" },
  { 0x13885, "Cisco SPA 525G" },
  { 0x13887, "Cisco SPA 509G" },
  { 0x13889, "Cisco SPA 525G2" },
  { 0x1388b, "Cisco SPA 303G" },
  { 0x00000, NULL }
};
static value_string_ext DeviceType_ext = VALUE_STRING_EXT_INIT(DeviceType);

static const value_string KeyPadButton[] = {
  { 0x00000, "Zero" },
  { 0x00001, "One" },
  { 0x00002, "Two" },
  { 0x00003, "Three" },
  { 0x00004, "Four" },
  { 0x00005, "Five" },
  { 0x00006, "Six" },
  { 0x00007, "Seven" },
  { 0x00008, "Eight" },
  { 0x00009, "Nine" },
  { 0x0000a, "A" },
  { 0x0000b, "B" },
  { 0x0000c, "C" },
  { 0x0000d, "D" },
  { 0x0000e, "Star" },
  { 0x0000f, "Pound" },
  { 0x00010, "Plus" },
  { 0x00000, NULL }
};
static value_string_ext KeyPadButton_ext = VALUE_STRING_EXT_INIT(KeyPadButton);

static const value_string DeviceStimulus[] = {
  { 0x00001, "LastNumberRedial" },
  { 0x00002, "SpeedDial" },
  { 0x00003, "Hold" },
  { 0x00004, "Transfer" },
  { 0x00005, "ForwardAll" },
  { 0x00006, "ForwardBusy" },
  { 0x00007, "ForwardNoAnswer" },
  { 0x00008, "Display" },
  { 0x00009, "Line" },
  { 0x0000a, "T120Chat" },
  { 0x0000b, "T120Whiteboard" },
  { 0x0000c, "T120ApplicationSharing" },
  { 0x0000d, "T120FileTransfer" },
  { 0x0000e, "Video" },
  { 0x0000f, "VoiceMail" },
  { 0x00010, "AnswerRelease" },
  { 0x00011, "AutoAnswer" },
  { 0x00012, "Select" },
  { 0x00013, "Privacy" },
  { 0x00014, "ServiceURL" },
  { 0x00015, "BLFSpeedDial" },
  { 0x00016, "DPark" },
  { 0x00017, "Intercom" },
  { 0x0001b, "MaliciousCall" },
  { 0x00021, "GenericAppB1" },
  { 0x00022, "GenericAppB2" },
  { 0x00023, "GenericAppB3" },
  { 0x00024, "GenericAppB4" },
  { 0x00025, "GenericAppB5" },
  { 0x0007b, "MeetMeConference" },
  { 0x0007d, "Conference" },
  { 0x0007e, "CallPark" },
  { 0x0007f, "CallPickUp" },
  { 0x00080, "GroupCallPickUp" },
  { 0x00081, "Mobility" },
  { 0x00082, "DoNotDisturb" },
  { 0x00083, "ConfList" },
  { 0x00084, "RemoveLastParticipant" },
  { 0x00085, "QRT" },
  { 0x00086, "CallBack" },
  { 0x00087, "OtherPickup" },
  { 0x00088, "VideoMode" },
  { 0x00089, "NewCall" },
  { 0x0008a, "EndCall" },
  { 0x0008b, "HLog" },
  { 0x0008f, "Queuing" },
  { 0x000ff, "MaxStimulusValue" },
  { 0x00000, NULL }
};
static value_string_ext DeviceStimulus_ext = VALUE_STRING_EXT_INIT(DeviceStimulus);

#define MEDIA_PAYLOAD_G711ALAW64K              0x00002 /* audio */
#define MEDIA_PAYLOAD_G711ALAW56K              0x00003 /* audio */
#define MEDIA_PAYLOAD_G711ULAW64K              0x00004 /* audio */
#define MEDIA_PAYLOAD_G711ULAW56K              0x00005 /* audio */
#define MEDIA_PAYLOAD_G722_64K                 0x00006 /* audio */
#define MEDIA_PAYLOAD_G722_56K                 0x00007 /* audio */
#define MEDIA_PAYLOAD_G722_48K                 0x00008 /* audio */
#define MEDIA_PAYLOAD_G7231                    0x00009 /* audio */
#define MEDIA_PAYLOAD_G728                     0x0000a /* audio */
#define MEDIA_PAYLOAD_G729                     0x0000b /* audio */
#define MEDIA_PAYLOAD_G729ANNEXA               0x0000c /* audio */
#define MEDIA_PAYLOAD_G729ANNEXB               0x0000f /* audio */
#define MEDIA_PAYLOAD_G729ANNEXAWANNEXB        0x00010 /* audio */
#define MEDIA_PAYLOAD_GSM_FULL_RATE            0x00012 /* audio */
#define MEDIA_PAYLOAD_GSM_HALF_RATE            0x00013 /* audio */
#define MEDIA_PAYLOAD_GSM_ENHANCED_FULL_RATE   0x00014 /* audio */
#define MEDIA_PAYLOAD_WIDE_BAND_256K           0x00019 /* audio */
#define MEDIA_PAYLOAD_DATA64                   0x00020 /* audio */
#define MEDIA_PAYLOAD_DATA56                   0x00021 /* audio */
#define MEDIA_PAYLOAD_G7221_32K                0x00028 /* audio */
#define MEDIA_PAYLOAD_G7221_24K                0x00029 /* audio */
#define MEDIA_PAYLOAD_AAC                      0x0002a /* audio */
#define MEDIA_PAYLOAD_MP4ALATM_128             0x0002b /* audio */
#define MEDIA_PAYLOAD_MP4ALATM_64              0x0002c /* audio */
#define MEDIA_PAYLOAD_MP4ALATM_56              0x0002d /* audio */
#define MEDIA_PAYLOAD_MP4ALATM_48              0x0002e /* audio */
#define MEDIA_PAYLOAD_MP4ALATM_32              0x0002f /* audio */
#define MEDIA_PAYLOAD_MP4ALATM_24              0x00030 /* audio */
#define MEDIA_PAYLOAD_MP4ALATM_NA              0x00031 /* audio */
#define MEDIA_PAYLOAD_GSM                      0x00050 /* audio */
#define MEDIA_PAYLOAD_G726_32K                 0x00052 /* audio */
#define MEDIA_PAYLOAD_G726_24K                 0x00053 /* audio */
#define MEDIA_PAYLOAD_G726_16K                 0x00054 /* audio */
#define MEDIA_PAYLOAD_ILBC                     0x00056 /* audio */
#define MEDIA_PAYLOAD_ISAC                     0x00059 /* audio */
#define MEDIA_PAYLOAD_OPUS                     0x0005a /* audio */
#define MEDIA_PAYLOAD_AMR                      0x00061 /* audio */
#define MEDIA_PAYLOAD_AMR_WB                   0x00062 /* audio */
#define MEDIA_PAYLOAD_H261                     0x00064 /* video */
#define MEDIA_PAYLOAD_H263                     0x00065 /* video */
#define MEDIA_PAYLOAD_VIEO                     0x00066 /* video */
#define MEDIA_PAYLOAD_H264                     0x00067 /* video */
#define MEDIA_PAYLOAD_H264_SVC                 0x00068 /* video */
#define MEDIA_PAYLOAD_T120                     0x00069 /* video */
#define MEDIA_PAYLOAD_H224                     0x0006a /* video */
#define MEDIA_PAYLOAD_T38FAX                   0x0006b /* video */
#define MEDIA_PAYLOAD_TOTE                     0x0006c /* video */
#define MEDIA_PAYLOAD_H265                     0x0006d /* video */
#define MEDIA_PAYLOAD_H264_UC                  0x0006e /* video */
#define MEDIA_PAYLOAD_XV150_MR_711U            0x0006f /* video */
#define MEDIA_PAYLOAD_NSE_VBD_711U             0x00070 /* video */
#define MEDIA_PAYLOAD_XV150_MR_729A            0x00071 /* video */
#define MEDIA_PAYLOAD_NSE_VBD_729A             0x00072 /* video */
#define MEDIA_PAYLOAD_H264_FEC                 0x00073 /* video */
#define MEDIA_PAYLOAD_CLEAR_CHAN               0x00078 /* data */
#define MEDIA_PAYLOAD_UNIVERSAL_XCODER         0x000de /* data */
#define MEDIA_PAYLOAD_RFC2833_DYNPAYLOAD       0x00101 /* data */
#define MEDIA_PAYLOAD_PASSTHROUGH              0x00102 /* data */
#define MEDIA_PAYLOAD_DYNAMIC_PAYLOAD_PASSTHRU 0x00103 /* data */
#define MEDIA_PAYLOAD_DTMF_OOB                 0x00104 /* data */
#define MEDIA_PAYLOAD_INBAND_DTMF_RFC2833      0x00105 /* data */
#define MEDIA_PAYLOAD_CFB_TONES                0x00106 /* data */
#define MEDIA_PAYLOAD_NOAUDIO                  0x0012b /* data */
#define MEDIA_PAYLOAD_V150_LC_MODEMRELAY       0x0012c /* data */
#define MEDIA_PAYLOAD_V150_LC_SPRT             0x0012d /* data */
#define MEDIA_PAYLOAD_V150_LC_SSE              0x0012e /* data */
#define MEDIA_PAYLOAD_MAX                      0x0012f /* data */

static const value_string Media_PayloadType[] = {
  { MEDIA_PAYLOAD_G711ALAW64K, "Media_Payload_G711Alaw64k" },
  { MEDIA_PAYLOAD_G711ALAW56K, "Media_Payload_G711Alaw56k" },
  { MEDIA_PAYLOAD_G711ULAW64K, "Media_Payload_G711Ulaw64k" },
  { MEDIA_PAYLOAD_G711ULAW56K, "Media_Payload_G711Ulaw56k" },
  { MEDIA_PAYLOAD_G722_64K, "Media_Payload_G722_64k" },
  { MEDIA_PAYLOAD_G722_56K, "Media_Payload_G722_56k" },
  { MEDIA_PAYLOAD_G722_48K, "Media_Payload_G722_48k" },
  { MEDIA_PAYLOAD_G7231, "Media_Payload_G7231" },
  { MEDIA_PAYLOAD_G728, "Media_Payload_G728" },
  { MEDIA_PAYLOAD_G729, "Media_Payload_G729" },
  { MEDIA_PAYLOAD_G729ANNEXA, "Media_Payload_G729AnnexA" },
  { MEDIA_PAYLOAD_G729ANNEXB, "Media_Payload_G729AnnexB" },
  { MEDIA_PAYLOAD_G729ANNEXAWANNEXB, "Media_Payload_G729AnnexAwAnnexB" },
  { MEDIA_PAYLOAD_GSM_FULL_RATE, "Media_Payload_GSM_Full_Rate" },
  { MEDIA_PAYLOAD_GSM_HALF_RATE, "Media_Payload_GSM_Half_Rate" },
  { MEDIA_PAYLOAD_GSM_ENHANCED_FULL_RATE, "Media_Payload_GSM_Enhanced_Full_Rate" },
  { MEDIA_PAYLOAD_WIDE_BAND_256K, "Media_Payload_Wide_Band_256k" },
  { MEDIA_PAYLOAD_DATA64, "Media_Payload_Data64" },
  { MEDIA_PAYLOAD_DATA56, "Media_Payload_Data56" },
  { MEDIA_PAYLOAD_G7221_32K, "Media_Payload_G7221_32K" },
  { MEDIA_PAYLOAD_G7221_24K, "Media_Payload_G7221_24K" },
  { MEDIA_PAYLOAD_AAC, "Media_Payload_AAC" },
  { MEDIA_PAYLOAD_MP4ALATM_128, "Media_Payload_MP4ALATM_128" },
  { MEDIA_PAYLOAD_MP4ALATM_64, "Media_Payload_MP4ALATM_64" },
  { MEDIA_PAYLOAD_MP4ALATM_56, "Media_Payload_MP4ALATM_56" },
  { MEDIA_PAYLOAD_MP4ALATM_48, "Media_Payload_MP4ALATM_48" },
  { MEDIA_PAYLOAD_MP4ALATM_32, "Media_Payload_MP4ALATM_32" },
  { MEDIA_PAYLOAD_MP4ALATM_24, "Media_Payload_MP4ALATM_24" },
  { MEDIA_PAYLOAD_MP4ALATM_NA, "Media_Payload_MP4ALATM_NA" },
  { MEDIA_PAYLOAD_GSM, "Media_Payload_GSM" },
  { MEDIA_PAYLOAD_G726_32K, "Media_Payload_G726_32K" },
  { MEDIA_PAYLOAD_G726_24K, "Media_Payload_G726_24K" },
  { MEDIA_PAYLOAD_G726_16K, "Media_Payload_G726_16K" },
  { MEDIA_PAYLOAD_ILBC, "Media_Payload_ILBC" },
  { MEDIA_PAYLOAD_ISAC, "Media_Payload_ISAC" },
  { MEDIA_PAYLOAD_OPUS, "Media_Payload_OPUS" },
  { MEDIA_PAYLOAD_AMR, "Media_Payload_AMR" },
  { MEDIA_PAYLOAD_AMR_WB, "Media_Payload_AMR_WB" },
  { MEDIA_PAYLOAD_H261, "Media_Payload_H261" },
  { MEDIA_PAYLOAD_H263, "Media_Payload_H263" },
  { MEDIA_PAYLOAD_VIEO, "Media_Payload_Vieo" },
  { MEDIA_PAYLOAD_H264, "Media_Payload_H264" },
  { MEDIA_PAYLOAD_H264_SVC, "Media_Payload_H264_SVC" },
  { MEDIA_PAYLOAD_T120, "Media_Payload_T120" },
  { MEDIA_PAYLOAD_H224, "Media_Payload_H224" },
  { MEDIA_PAYLOAD_T38FAX, "Media_Payload_T38Fax" },
  { MEDIA_PAYLOAD_TOTE, "Media_Payload_TOTE" },
  { MEDIA_PAYLOAD_H265, "Media_Payload_H265" },
  { MEDIA_PAYLOAD_H264_UC, "Media_Payload_H264_UC" },
  { MEDIA_PAYLOAD_XV150_MR_711U, "Media_Payload_XV150_MR_711U" },
  { MEDIA_PAYLOAD_NSE_VBD_711U, "Media_Payload_NSE_VBD_711U" },
  { MEDIA_PAYLOAD_XV150_MR_729A, "Media_Payload_XV150_MR_729A" },
  { MEDIA_PAYLOAD_NSE_VBD_729A, "Media_Payload_NSE_VBD_729A" },
  { MEDIA_PAYLOAD_H264_FEC, "Media_Payload_H264_FEC" },
  { MEDIA_PAYLOAD_CLEAR_CHAN, "Media_Payload_Clear_Chan" },
  { MEDIA_PAYLOAD_UNIVERSAL_XCODER, "Media_Payload_Universal_Xcoder" },
  { MEDIA_PAYLOAD_RFC2833_DYNPAYLOAD, "Media_Payload_RFC2833_DynPayload" },
  { MEDIA_PAYLOAD_PASSTHROUGH, "Media_Payload_PassThrough" },
  { MEDIA_PAYLOAD_DYNAMIC_PAYLOAD_PASSTHRU, "Media_Payload_Dynamic_Payload_PassThru" },
  { MEDIA_PAYLOAD_DTMF_OOB, "Media_Payload_DTMF_OOB" },
  { MEDIA_PAYLOAD_INBAND_DTMF_RFC2833, "Media_Payload_Inband_DTMF_RFC2833" },
  { MEDIA_PAYLOAD_CFB_TONES, "Media_Payload_CFB_Tones" },
  { MEDIA_PAYLOAD_NOAUDIO, "Media_Payload_NoAudio" },
  { MEDIA_PAYLOAD_V150_LC_MODEMRELAY, "Media_Payload_v150_LC_ModemRelay" },
  { MEDIA_PAYLOAD_V150_LC_SPRT, "Media_Payload_v150_LC_SPRT" },
  { MEDIA_PAYLOAD_V150_LC_SSE, "Media_Payload_v150_LC_SSE" },
  { MEDIA_PAYLOAD_MAX, "Media_Payload_Max" },
  { 0x00000, NULL }
};
static value_string_ext Media_PayloadType_ext = VALUE_STRING_EXT_INIT(Media_PayloadType);

static const value_string Media_G723BitRate[] = {
  { 0x00001, "Media_G723BRate_5_3" },
  { 0x00002, "Media_G723BRate_6_3" },
  { 0x00000, NULL }
};
static value_string_ext Media_G723BitRate_ext = VALUE_STRING_EXT_INIT(Media_G723BitRate);

static const value_string DeviceAlarmSeverity[] = {
  { 0x00000, "Critical" },
  { 0x00001, "Warning" },
  { 0x00002, "Informational" },
  { 0x00004, "Unknown" },
  { 0x00007, "Major" },
  { 0x00008, "Minor" },
  { 0x0000a, "Marginal" },
  { 0x00014, "TraceInfo" },
  { 0x00000, NULL }
};
static value_string_ext DeviceAlarmSeverity_ext = VALUE_STRING_EXT_INIT(DeviceAlarmSeverity);

static const value_string MulticastMediaReceptionStatus[] = {
  { 0x00000, "Ok" },
  { 0x00001, "Error" },
  { 0x00000, NULL }
};
static value_string_ext MulticastMediaReceptionStatus_ext = VALUE_STRING_EXT_INIT(MulticastMediaReceptionStatus);

static const value_string MediaStatus[] = {
  { 0x00000, "Ok" },
  { 0x00001, "Unknown" },
  { 0x00002, "NotEnoughChannels" },
  { 0x00003, "CodecTooComplex" },
  { 0x00004, "InvalidPartyID" },
  { 0x00005, "InvalidCallRef" },
  { 0x00006, "InvalidCodec" },
  { 0x00007, "InvalidPacketSize" },
  { 0x00008, "OutOfSockets" },
  { 0x00009, "EncoderOrDecoderFailed" },
  { 0x0000a, "InvalidDynamicPayloadType" },
  { 0x0000b, "RequestedIpAddrTypeUnAvailable" },
  { 0x000ff, "DeviceOnHook" },
  { 0x00000, NULL }
};
static value_string_ext MediaStatus_ext = VALUE_STRING_EXT_INIT(MediaStatus);

#define IPADDRTYPE_IPV4                        0x00000
#define IPADDRTYPE_IPV6                        0x00001
#define IPADDRTYPE_IPV4_V6                     0x00002
#define IPADDRTYPE_IP_INVALID                  0x00003

static const value_string IpAddrType[] = {
  { IPADDRTYPE_IPV4, "v4" },
  { IPADDRTYPE_IPV6, "v6" },
  { IPADDRTYPE_IPV4_V6, "v4_v6" },
  { IPADDRTYPE_IP_INVALID, "_Invalid" },
  { 0x00000, NULL }
};
static value_string_ext IpAddrType_ext = VALUE_STRING_EXT_INIT(IpAddrType);

static const value_string StatsProcessingType[] = {
  { 0x00000, "clearStats" },
  { 0x00001, "doNotClearStats" },
  { 0x00000, NULL }
};
static value_string_ext StatsProcessingType_ext = VALUE_STRING_EXT_INIT(StatsProcessingType);

static const value_string SoftKeySet[] = {
  { 0x00000, "On Hook" },
  { 0x00001, "Connected" },
  { 0x00002, "On Hold" },
  { 0x00003, "Ring In" },
  { 0x00004, "Off Hook" },
  { 0x00005, "Connected Transferable" },
  { 0x00006, "Digits Following" },
  { 0x00007, "Connected Conference" },
  { 0x00008, "Ring Out" },
  { 0x00009, "OffHook with Features" },
  { 0x0000a, "In Use Hint" },
  { 0x0000b, "On Hook with Stealable Call" },
  { 0x00000, NULL }
};
static value_string_ext SoftKeySet_ext = VALUE_STRING_EXT_INIT(SoftKeySet);

static const value_string SoftKeyEvent[] = {
  { 0x00001, "Redial" },
  { 0x00002, "NewCall" },
  { 0x00003, "Hold" },
  { 0x00004, "Transfer" },
  { 0x00005, "CfwdAll" },
  { 0x00006, "CfwdBusy" },
  { 0x00007, "CfwdNoAnswer" },
  { 0x00008, "BackSpace" },
  { 0x00009, "EndCall" },
  { 0x0000a, "Resume" },
  { 0x0000b, "Answer" },
  { 0x0000c, "Info" },
  { 0x0000d, "Confrn" },
  { 0x0000e, "Park" },
  { 0x0000f, "Join" },
  { 0x00010, "MeetMe" },
  { 0x00011, "PickUp" },
  { 0x00012, "GrpPickup" },
  { 0x00013, "Your current options" },
  { 0x00014, "Off Hook" },
  { 0x00015, "On Hook" },
  { 0x00016, "Ring out" },
  { 0x00017, "From " },
  { 0x00018, "Connected" },
  { 0x00019, "Busy" },
  { 0x0001a, "Line In Use" },
  { 0x0001b, "Call Waiting" },
  { 0x0001c, "Call Transfer" },
  { 0x0001d, "Call Park" },
  { 0x0001e, "Call Proceed" },
  { 0x0001f, "In Use Remote" },
  { 0x00020, "Enter number" },
  { 0x00021, "Call park At" },
  { 0x00022, "Primary Only" },
  { 0x00023, "Temp Fail" },
  { 0x00024, "You Have a VoiceMail" },
  { 0x00025, "Forwarded to" },
  { 0x00026, "Can Not Complete Conference" },
  { 0x00027, "No Conference Bridge" },
  { 0x00028, "Can Not Hold Primary Control" },
  { 0x00029, "Invalid Conference Participant" },
  { 0x0002a, "In Conference Already" },
  { 0x0002b, "No Participant Info" },
  { 0x0002c, "Exceed Maximum Parties" },
  { 0x0002d, "Key Is Not Active" },
  { 0x0002e, "Error No License" },
  { 0x0002f, "Error DBConfig" },
  { 0x00030, "Error Database" },
  { 0x00031, "Error Pass Limit" },
  { 0x00032, "Error Unknown" },
  { 0x00033, "Error Mismatch" },
  { 0x00034, "Conference" },
  { 0x00035, "Park Number" },
  { 0x00036, "Private" },
  { 0x00037, "Not Enough Bandwidth" },
  { 0x00038, "Unknown Number" },
  { 0x00039, "RmLstC" },
  { 0x0003a, "Voicemail" },
  { 0x0003b, "ImmDiv" },
  { 0x0003c, "Intrcpt" },
  { 0x0003d, "SetWtch" },
  { 0x0003e, "TrnsfVM" },
  { 0x0003f, "DND" },
  { 0x00040, "DivAll" },
  { 0x00041, "CallBack" },
  { 0x00042, "Network congestion,rerouting" },
  { 0x00043, "Barge" },
  { 0x00044, "Failed to setup Barge" },
  { 0x00045, "Another Barge exists" },
  { 0x00046, "Incompatible device type" },
  { 0x00047, "No Park Number Available" },
  { 0x00048, "CallPark Reversion" },
  { 0x00049, "Service is not Active" },
  { 0x0004a, "High Traffic Try Again Later" },
  { 0x0004b, "QRT" },
  { 0x0004c, "MCID" },
  { 0x0004d, "DirTrfr" },
  { 0x0004e, "Select" },
  { 0x0004f, "ConfList" },
  { 0x00050, "iDivert" },
  { 0x00051, "cBarge" },
  { 0x00052, "Can Not Complete Transfer" },
  { 0x00053, "Can Not Join Calls" },
  { 0x00054, "Mcid Successful" },
  { 0x00055, "Number Not Configured" },
  { 0x00056, "Security Error" },
  { 0x00057, "Video Bandwidth Unavailable" },
  { 0x00058, "Video Mode" },
  { 0x000c9, "Dial" },
  { 0x000ca, "Record" },
  { 0x00000, NULL }
};
static value_string_ext SoftKeyEvent_ext = VALUE_STRING_EXT_INIT(SoftKeyEvent);

static const value_string UnRegReasonCode[] = {
  { 0x00000, "Unknown" },
  { 0x00001, "PowerSaveMode" },
  { 0x00000, NULL }
};
static value_string_ext UnRegReasonCode_ext = VALUE_STRING_EXT_INIT(UnRegReasonCode);

static const value_string HeadsetMode[] = {
  { 0x00001, "On" },
  { 0x00002, "Off" },
  { 0x00000, NULL }
};
static value_string_ext HeadsetMode_ext = VALUE_STRING_EXT_INIT(HeadsetMode);

static const value_string SequenceFlag[] = {
  { 0x00000, "First" },
  { 0x00001, "More" },
  { 0x00002, "Last" },
  { 0x00000, NULL }
};
static value_string_ext SequenceFlag_ext = VALUE_STRING_EXT_INIT(SequenceFlag);

static const value_string Layout[] = {
  { 0x00000, "NoLayout" },
  { 0x00001, "OneByOne" },
  { 0x00002, "OneByTwo" },
  { 0x00003, "TwoByTwo" },
  { 0x00004, "TwoByTwo3Alt1" },
  { 0x00005, "TwoByTwo3Alt2" },
  { 0x00006, "ThreeByThree" },
  { 0x00007, "ThreeByThree6Alt1" },
  { 0x00008, "ThreeByThree6Alt2" },
  { 0x00009, "ThreeByThree4Alt1" },
  { 0x0000a, "ThreeByThree4Alt2" },
  { 0x00000, NULL }
};
static value_string_ext Layout_ext = VALUE_STRING_EXT_INIT(Layout);

static const value_string TransmitOrReceive[] = {
  { 0x00000, "None" },
  { 0x00001, "ReceiveOnly" },
  { 0x00002, "TransmitOnly" },
  { 0x00003, "Both" },
  { 0x00000, NULL }
};
static value_string_ext TransmitOrReceive_ext = VALUE_STRING_EXT_INIT(TransmitOrReceive);

static const value_string OpenReceiveChanStatus[] = {
  { 0x00000, "Ok" },
  { 0x00001, "Error" },
  { 0x00000, NULL }
};
static value_string_ext OpenReceiveChanStatus_ext = VALUE_STRING_EXT_INIT(OpenReceiveChanStatus);

static const value_string CreateConfResult[] = {
  { 0x00000, "OK" },
  { 0x00001, "ResourceNotAvailable" },
  { 0x00002, "ConferenceAlreadyExist" },
  { 0x00003, "SystemErr" },
  { 0x00000, NULL }
};
static value_string_ext CreateConfResult_ext = VALUE_STRING_EXT_INIT(CreateConfResult);

static const value_string DeleteConfResult[] = {
  { 0x00000, "OK" },
  { 0x00001, "ConferenceNotExist" },
  { 0x00002, "SystemErr" },
  { 0x00000, NULL }
};
static value_string_ext DeleteConfResult_ext = VALUE_STRING_EXT_INIT(DeleteConfResult);

static const value_string ModifyConfResult[] = {
  { 0x00000, "OK" },
  { 0x00001, "ResourceNotAvailable" },
  { 0x00002, "ConferenceNotExist" },
  { 0x00003, "InvalidParameter" },
  { 0x00004, "MoreActiveCallsThanReserved" },
  { 0x00005, "InvalidResourceType" },
  { 0x00006, "SystemErr" },
  { 0x00000, NULL }
};
static value_string_ext ModifyConfResult_ext = VALUE_STRING_EXT_INIT(ModifyConfResult);

static const value_string AddParticipantResult[] = {
  { 0x00000, "OK" },
  { 0x00001, "ResourceNotAvailable" },
  { 0x00002, "ConferenceNotExist" },
  { 0x00003, "DuplicateCallRef" },
  { 0x00004, "SystemErr" },
  { 0x00000, NULL }
};
static value_string_ext AddParticipantResult_ext = VALUE_STRING_EXT_INIT(AddParticipantResult);

static const value_string ResourceType[] = {
  { 0x00000, "Conference" },
  { 0x00001, "IVR" },
  { 0x00000, NULL }
};
static value_string_ext ResourceType_ext = VALUE_STRING_EXT_INIT(ResourceType);

static const value_string AuditParticipantResult[] = {
  { 0x00000, "OK" },
  { 0x00001, "ConferenceNotExist" },
  { 0x00000, NULL }
};
static value_string_ext AuditParticipantResult_ext = VALUE_STRING_EXT_INIT(AuditParticipantResult);

static const value_string Media_Encryption_Capability[] = {
  { 0x00000, "NotEncryptionCapable" },
  { 0x00001, "EncryptionCapable" },
  { 0x00000, NULL }
};
static value_string_ext Media_Encryption_Capability_ext = VALUE_STRING_EXT_INIT(Media_Encryption_Capability);

static const value_string IpAddrMode[] = {
  { 0x00000, "ModeIpv4" },
  { 0x00001, "ModeIpv6" },
  { 0x00002, "ModeIpv4AndIpv6" },
  { 0x00000, NULL }
};
static value_string_ext IpAddrMode_ext = VALUE_STRING_EXT_INIT(IpAddrMode);

static const value_string MediaType[] = {
  { 0x00000, "MediaType_Invalid" },
  { 0x00001, "MediaType_Audio" },
  { 0x00002, "MediaType_Main_Video" },
  { 0x00003, "MediaType_FECC" },
  { 0x00004, "MediaType_Presentation_Video" },
  { 0x00005, "MediaType_DataApp_BFCP" },
  { 0x00006, "MediaType_DataApp_IXChannel" },
  { 0x00007, "MediaType_T38" },
  { 0x00008, "MediaType_Max" },
  { 0x00000, NULL }
};
static value_string_ext MediaType_ext = VALUE_STRING_EXT_INIT(MediaType);

static const value_string RSVPDirection[] = {
  { 0x00001, "SEND" },
  { 0x00002, "RECV" },
  { 0x00003, "SENDRECV" },
  { 0x00000, NULL }
};
static value_string_ext RSVPDirection_ext = VALUE_STRING_EXT_INIT(RSVPDirection);

static const value_string QoSErrorCode[] = {
  { 0x00000, "QOS_CAUSE_RESERVATION_TIMEOUT" },
  { 0x00001, "QOS_CAUSE_PATH_FAIL" },
  { 0x00002, "QOS_CAUSE_RESV_FAIL" },
  { 0x00003, "QOS_CAUSE_LISTEN_FAIL" },
  { 0x00004, "QOS_CAUSE_RESOURCE_UNAVAILABLE" },
  { 0x00005, "QOS_CAUSE_LISTEN_TIMEOUT" },
  { 0x00006, "QOS_CAUSE_RESV_RETRIES_FAIL" },
  { 0x00007, "QOS_CAUSE_PATH_RETRIES_FAIL" },
  { 0x00008, "QOS_CAUSE_RESV_PREEMPTION" },
  { 0x00009, "QOS_CAUSE_PATH_PREEMPTION" },
  { 0x0000a, "QOS_CAUSE_RESV_MODIFY_FAIL" },
  { 0x0000b, "QOS_CAUSE_PATH_MODIFY_FAIL" },
  { 0x0000c, "QOS_CAUSE_RESV_TEAR" },
  { 0x00000, NULL }
};
static value_string_ext QoSErrorCode_ext = VALUE_STRING_EXT_INIT(QoSErrorCode);

static const value_string RSVPErrorCode[] = {
  { 0x00000, "CONFIRM" },
  { 0x00001, "ADMISSION" },
  { 0x00002, "ADMINISTRATIVE" },
  { 0x00003, "NO_PATH_INFORMATION" },
  { 0x00004, "NO_SENDER_INFORMATION" },
  { 0x00005, "CONFLICTING_STYLE" },
  { 0x00006, "UNKNOWN_STYLE" },
  { 0x00007, "CONFLICTING_DST_PORTS" },
  { 0x00008, "CONFLICTING_SRC_PORTS" },
  { 0x0000c, "SERVICE_PREEMPTED" },
  { 0x0000d, "UNKNOWN_OBJECT_CLASS" },
  { 0x0000e, "UNKNOWN_CLASS_TYPE" },
  { 0x00014, "API" },
  { 0x00015, "TRAFFIC" },
  { 0x00016, "TRAFFIC_SYSTEM" },
  { 0x00017, "SYSTEM" },
  { 0x00018, "ROUTING_PROBLEM" },
  { 0x00000, NULL }
};
static value_string_ext RSVPErrorCode_ext = VALUE_STRING_EXT_INIT(RSVPErrorCode);

static const value_string SubscriptionFeatureID[] = {
  { 0x00001, "BLF" },
  { 0x00000, NULL }
};
static value_string_ext SubscriptionFeatureID_ext = VALUE_STRING_EXT_INIT(SubscriptionFeatureID);

static const value_string MediaPathID[] = {
  { 0x00001, "Headset" },
  { 0x00002, "Handset" },
  { 0x00003, "Speaker" },
  { 0x00000, NULL }
};
static value_string_ext MediaPathID_ext = VALUE_STRING_EXT_INIT(MediaPathID);

static const value_string MediaPathEvent[] = {
  { 0x00001, "On" },
  { 0x00002, "Off" },
  { 0x00000, NULL }
};
static value_string_ext MediaPathEvent_ext = VALUE_STRING_EXT_INIT(MediaPathEvent);

static const value_string MediaPathCapabilities[] = {
  { 0x00001, "Enable" },
  { 0x00002, "Disable" },
  { 0x00003, "Monitor" },
  { 0x00000, NULL }
};
static value_string_ext MediaPathCapabilities_ext = VALUE_STRING_EXT_INIT(MediaPathCapabilities);

static const value_string DeviceTone[] = {
  { 0x00000, "Silence" },
  { 0x00001, "Dtmf1" },
  { 0x00002, "Dtmf2" },
  { 0x00003, "Dtmf3" },
  { 0x00004, "Dtmf4" },
  { 0x00005, "Dtmf5" },
  { 0x00006, "Dtmf6" },
  { 0x00007, "Dtmf7" },
  { 0x00008, "Dtmf8" },
  { 0x00009, "Dtmf9" },
  { 0x0000a, "Dtmf0" },
  { 0x0000e, "DtmfStar" },
  { 0x0000f, "DtmfPound" },
  { 0x00010, "DtmfA" },
  { 0x00011, "DtmfB" },
  { 0x00012, "DtmfC" },
  { 0x00013, "DtmfD" },
  { 0x00021, "InsideDialTone" },
  { 0x00022, "OutsideDialTone" },
  { 0x00023, "LineBusyTone" },
  { 0x00024, "AlertingTone" },
  { 0x00025, "ReorderTone" },
  { 0x00026, "RecorderWarningTone" },
  { 0x00027, "RecorderDetectedTone" },
  { 0x00028, "RevertingTone" },
  { 0x00029, "ReceiverOffHookTone" },
  { 0x0002a, "MessageWaitingIndicatorTone" },
  { 0x0002b, "NoSuchNumberTone" },
  { 0x0002c, "BusyVerificationTone" },
  { 0x0002d, "CallWaitingTone" },
  { 0x0002e, "ConfirmationTone" },
  { 0x0002f, "CampOnIndicationTone" },
  { 0x00030, "RecallDialTone" },
  { 0x00031, "ZipZip" },
  { 0x00032, "Zip" },
  { 0x00033, "BeepBonk" },
  { 0x00034, "MusicTone" },
  { 0x00035, "HoldTone" },
  { 0x00036, "TestTone" },
  { 0x00038, "MonitorWarningTone" },
  { 0x00039, "SecureWarningTone" },
  { 0x00040, "AddCallWaiting" },
  { 0x00041, "PriorityCallWait" },
  { 0x00042, "RecallDial" },
  { 0x00043, "BargIn" },
  { 0x00044, "DistinctAlert" },
  { 0x00045, "PriorityAlert" },
  { 0x00046, "ReminderRing" },
  { 0x00047, "PrecedenceRingBack" },
  { 0x00048, "PreemptionTone" },
  { 0x00049, "NonSecureWarningTone" },
  { 0x00050, "MF1" },
  { 0x00051, "MF2" },
  { 0x00052, "MF3" },
  { 0x00053, "MF4" },
  { 0x00054, "MF5" },
  { 0x00055, "MF6" },
  { 0x00056, "MF7" },
  { 0x00057, "MF8" },
  { 0x00058, "MF9" },
  { 0x00059, "MF0" },
  { 0x0005a, "MFKP1" },
  { 0x0005b, "MFST" },
  { 0x0005c, "MFKP2" },
  { 0x0005d, "MFSTP" },
  { 0x0005e, "MFST3P" },
  { 0x0005f, "MILLIWATT" },
  { 0x00060, "MILLIWATTTEST" },
  { 0x00061, "HIGHTONE" },
  { 0x00062, "FLASHOVERRIDE" },
  { 0x00063, "FLASH" },
  { 0x00064, "PRIORITY" },
  { 0x00065, "IMMEDIATE" },
  { 0x00066, "PREAMPWARN" },
  { 0x00067, "2105HZ" },
  { 0x00068, "2600HZ" },
  { 0x00069, "440HZ" },
  { 0x0006a, "300HZ" },
  { 0x0006b, "Mobility_WP" },
  { 0x0006c, "Mobility_UAC" },
  { 0x0006d, "Mobility_WTDN" },
  { 0x0006e, "Mobility_MON" },
  { 0x0006f, "Mobility_MOFF" },
  { 0x00070, "Mobility_UKC" },
  { 0x00071, "Mobility_VMA" },
  { 0x00072, "Mobility_FAC" },
  { 0x00073, "Mobility_CMC" },
  { 0x00077, "MLPP_PALA" },
  { 0x00078, "MLPP_ICA" },
  { 0x00079, "MLPP_VCA" },
  { 0x0007a, "MLPP_BPA" },
  { 0x0007b, "MLPP_BNEA" },
  { 0x0007c, "MLPP_UPA" },
  { 0x0007d, "TUA" },
  { 0x0007e, "GONE" },
  { 0x0007f, "NoTone" },
  { 0x00080, "MeetMe_Greeting" },
  { 0x00081, "MeetMe_NumberInvalid" },
  { 0x00082, "MeetMe_NumberFailed" },
  { 0x00083, "MeetMe_EnterPIN" },
  { 0x00084, "MeetMe_InvalidPIN" },
  { 0x00085, "MeetMe_FailedPIN" },
  { 0x00086, "MeetMe_CFB_Failed" },
  { 0x00087, "MeetMe_EnterAccessCode" },
  { 0x00088, "MeetMe_AccessCodeInvalid" },
  { 0x00089, "MeetMe_AccessCodeFailed" },
  { 0x0008a, "MAX" },
  { 0x00000, NULL }
};
static value_string_ext DeviceTone_ext = VALUE_STRING_EXT_INIT(DeviceTone);

static const value_string ToneOutputDirection[] = {
  { 0x00000, "User" },
  { 0x00001, "Network" },
  { 0x00002, "All" },
  { 0x00000, NULL }
};
static value_string_ext ToneOutputDirection_ext = VALUE_STRING_EXT_INIT(ToneOutputDirection);

static const value_string RingMode[] = {
  { 0x00001, "RingOff" },
  { 0x00002, "InsideRing" },
  { 0x00003, "OutsideRing" },
  { 0x00004, "FeatureRing" },
  { 0x00005, "FlashOnly" },
  { 0x00006, "PrecedenceRing" },
  { 0x00000, NULL }
};
static value_string_ext RingMode_ext = VALUE_STRING_EXT_INIT(RingMode);

static const value_string RingDuration[] = {
  { 0x00001, "NormalRing" },
  { 0x00002, "SingleRing" },
  { 0x00000, NULL }
};
static value_string_ext RingDuration_ext = VALUE_STRING_EXT_INIT(RingDuration);

static const value_string LampMode[] = {
  { 0x00001, "Off" },
  { 0x00002, "On" },
  { 0x00003, "Wink" },
  { 0x00004, "Flash" },
  { 0x00005, "Blink" },
  { 0x00000, NULL }
};
static value_string_ext LampMode_ext = VALUE_STRING_EXT_INIT(LampMode);

static const value_string SpeakerMode[] = {
  { 0x00001, "On" },
  { 0x00002, "Off" },
  { 0x00000, NULL }
};
static value_string_ext SpeakerMode_ext = VALUE_STRING_EXT_INIT(SpeakerMode);

static const value_string MicrophoneMode[] = {
  { 0x00001, "On" },
  { 0x00002, "Off" },
  { 0x00000, NULL }
};
static value_string_ext MicrophoneMode_ext = VALUE_STRING_EXT_INIT(MicrophoneMode);

static const value_string Media_SilenceSuppression[] = {
  { 0x00000, "Media_SilenceSuppression_Off" },
  { 0x00001, "Media_SilenceSuppression_On" },
  { 0x00000, NULL }
};
static value_string_ext Media_SilenceSuppression_ext = VALUE_STRING_EXT_INIT(Media_SilenceSuppression);

static const value_string MediaEncryptionAlgorithmType[] = {
  { 0x00000, "NO_ENCRYPTION" },
  { 0x00001, "CCM_AES_CM_128_HMAC_SHA1_32" },
  { 0x00002, "CCM_AES_CM_128_HMAC_SHA1_80" },
  { 0x00003, "CCM_F8_128_HMAC_SHA1_32" },
  { 0x00004, "CCM_F8_128_HMAC_SHA1_80" },
  { 0x00005, "CCM_AEAD_AES_128_GCM" },
  { 0x00006, "CCM_AEAD_AES_256_GCM" },
  { 0x00000, NULL }
};
static value_string_ext MediaEncryptionAlgorithmType_ext = VALUE_STRING_EXT_INIT(MediaEncryptionAlgorithmType);

static const value_string PortHandling[] = {
  { 0x00000, "CLOSE_PORT" },
  { 0x00001, "KEEP_PORT" },
  { 0x00000, NULL }
};
static value_string_ext PortHandling_ext = VALUE_STRING_EXT_INIT(PortHandling);

static const value_string CallType[] = {
  { 0x00001, "InBoundCall" },
  { 0x00002, "OutBoundCall" },
  { 0x00003, "ForwardCall" },
  { 0x00000, NULL }
};
static value_string_ext CallType_ext = VALUE_STRING_EXT_INIT(CallType);

static const value_string CallSecurityStatusType[] = {
  { 0x00000, "Unknown" },
  { 0x00001, "NotAuthenticated" },
  { 0x00002, "Authenticated" },
  { 0x00003, "Encrypted" },
  { 0x00004, "Max" },
  { 0x00000, NULL }
};
static value_string_ext CallSecurityStatusType_ext = VALUE_STRING_EXT_INIT(CallSecurityStatusType);

static const value_string SessionType[] = {
  { 0x00001, "Chat" },
  { 0x00002, "Whiteboard" },
  { 0x00004, "ApplicationSharing" },
  { 0x00008, "FileTransfer" },
  { 0x00010, "Video" },
  { 0x00000, NULL }
};
static value_string_ext SessionType_ext = VALUE_STRING_EXT_INIT(SessionType);

static const value_string ButtonType[] = {
  { 0x00000, "Unused" },
  { 0x00001, "Last Number Redial" },
  { 0x00002, "SpeedDial" },
  { 0x00003, "Hold" },
  { 0x00004, "Transfer" },
  { 0x00005, "Forward All" },
  { 0x00006, "Forward Busy" },
  { 0x00007, "Forward No Answer" },
  { 0x00008, "Display" },
  { 0x00009, "Line" },
  { 0x0000a, "T120 Chat" },
  { 0x0000b, "T120 Whiteboard" },
  { 0x0000c, "T120 Application Sharing" },
  { 0x0000d, "T120 File Transfer" },
  { 0x0000e, "Video" },
  { 0x0000f, "Voicemail" },
  { 0x00010, "Answer Release" },
  { 0x00011, "Auto Answer" },
  { 0x00012, "Select" },
  { 0x00013, "Feature" },
  { 0x00014, "ServiceURL" },
  { 0x00015, "BusyLampField Speeddial" },
  { 0x0001b, "Malicious Call" },
  { 0x00021, "Generic App B1" },
  { 0x00022, "Generic App B2" },
  { 0x00023, "Generic App B3" },
  { 0x00024, "Generic App B4" },
  { 0x00025, "Generic App B5" },
  { 0x00026, "Monitor/Multiblink" },
  { 0x0007b, "Meet Me Conference" },
  { 0x0007d, "Conference" },
  { 0x0007e, "Call Park" },
  { 0x0007f, "Call Pickup" },
  { 0x00080, "Group Call Pickup" },
  { 0x00081, "Mobility" },
  { 0x00082, "DoNotDisturb" },
  { 0x00083, "ConfList" },
  { 0x00084, "RemoveLastParticipant" },
  { 0x00085, "QRT" },
  { 0x00086, "CallBack" },
  { 0x00087, "OtherPickup" },
  { 0x00088, "VideoMode" },
  { 0x00089, "NewCall" },
  { 0x0008a, "EndCall" },
  { 0x0008b, "HLog" },
  { 0x0008f, "Queuing" },
  { 0x000c0, "Test E" },
  { 0x000c1, "Test F" },
  { 0x000c2, "Messages" },
  { 0x000c3, "Directory" },
  { 0x000c4, "Test I" },
  { 0x000c5, "Application" },
  { 0x000c6, "Headset" },
  { 0x000f0, "Keypad" },
  { 0x000fd, "Aec" },
  { 0x000ff, "Undefined" },
  { 0x00000, NULL }
};
static value_string_ext ButtonType_ext = VALUE_STRING_EXT_INIT(ButtonType);

static const value_string DeviceResetType[] = {
  { 0x00001, "RESET" },
  { 0x00002, "RESTART" },
  { 0x00003, "APPLY_CONFIG" },
  { 0x00000, NULL }
};
static value_string_ext DeviceResetType_ext = VALUE_STRING_EXT_INIT(DeviceResetType);

static const value_string Media_EchoCancellation[] = {
  { 0x00000, "Media_EchoCancellation_Off" },
  { 0x00001, "Media_EchoCancellation_On" },
  { 0x00000, NULL }
};
static value_string_ext Media_EchoCancellation_ext = VALUE_STRING_EXT_INIT(Media_EchoCancellation);

static const value_string SoftKeyTemplateIndex[] = {
  { 0x00001, "Redial" },
  { 0x00002, "NewCall" },
  { 0x00003, "Hold" },
  { 0x00004, "Transfer" },
  { 0x00005, "CfwdAll" },
  { 0x00006, "CfwdBusy" },
  { 0x00007, "CfwdNoAnswer" },
  { 0x00008, "BackSpace" },
  { 0x00009, "EndCall" },
  { 0x0000a, "Resume" },
  { 0x0000b, "Answer" },
  { 0x0000c, "Info" },
  { 0x0000d, "Confrn" },
  { 0x0000e, "Park" },
  { 0x0000f, "Join" },
  { 0x00010, "MeetMe" },
  { 0x00011, "PickUp" },
  { 0x00012, "GrpPickup" },
  { 0x00013, "Monitor" },
  { 0x00014, "CallBack" },
  { 0x00015, "Barge" },
  { 0x00016, "DND" },
  { 0x00017, "ConfList" },
  { 0x00018, "Select" },
  { 0x00019, "Private" },
  { 0x0001a, "Transfer Voicemail" },
  { 0x0001b, "Direct Transfer" },
  { 0x0001c, "Immediate Divert" },
  { 0x0001d, "Video Mode" },
  { 0x0001e, "Intercept" },
  { 0x0001f, "Empty" },
  { 0x00020, "Dial" },
  { 0x00021, "Conference Barge" },
  { 0x00000, NULL }
};
static value_string_ext SoftKeyTemplateIndex_ext = VALUE_STRING_EXT_INIT(SoftKeyTemplateIndex);

static const value_string SoftKeyInfoIndex[] = {
  { 0x0012d, "Redial" },
  { 0x0012e, "NewCall" },
  { 0x0012f, "Hold" },
  { 0x00130, "Transfer" },
  { 0x00131, "CfwdAll" },
  { 0x00132, "CfwdBusy" },
  { 0x00133, "CfwdNoAnswer" },
  { 0x00134, "BackSpace" },
  { 0x00135, "EndCall" },
  { 0x00136, "Resume" },
  { 0x00137, "Answer" },
  { 0x00138, "Info" },
  { 0x00139, "Confrn" },
  { 0x0013a, "Park" },
  { 0x0013b, "Join" },
  { 0x0013c, "MeetMe" },
  { 0x0013d, "PickUp" },
  { 0x0013e, "GrpPickup" },
  { 0x0013f, "Monitor" },
  { 0x00140, "CallBack" },
  { 0x00141, "Barge" },
  { 0x00142, "DND" },
  { 0x00143, "ConfList" },
  { 0x00144, "Select" },
  { 0x00145, "Private" },
  { 0x00146, "Transfer Voicemail" },
  { 0x00147, "Direct Transfer" },
  { 0x00148, "Immediate Divert" },
  { 0x00149, "Video Mode" },
  { 0x0014a, "Intercept" },
  { 0x0014b, "Empty" },
  { 0x0014c, "Dial" },
  { 0x0014d, "Conference Barge" },
  { 0x00000, NULL }
};
static value_string_ext SoftKeyInfoIndex_ext = VALUE_STRING_EXT_INIT(SoftKeyInfoIndex);

static const value_string DCallState[] = {
  { 0x00000, "Idle" },
  { 0x00001, "OffHook" },
  { 0x00002, "OnHook" },
  { 0x00003, "RingOut" },
  { 0x00004, "RingIn" },
  { 0x00005, "Connected" },
  { 0x00006, "Busy" },
  { 0x00007, "Congestion" },
  { 0x00008, "Hold" },
  { 0x00009, "CallWaiting" },
  { 0x0000a, "CallTransfer" },
  { 0x0000b, "CallPark" },
  { 0x0000c, "Proceed" },
  { 0x0000d, "CallRemoteMultiline" },
  { 0x0000e, "InvalidNumber" },
  { 0x0000f, "HoldRevert" },
  { 0x00010, "Whisper" },
  { 0x00011, "RemoteHold" },
  { 0x00012, "MaxState" },
  { 0x00000, NULL }
};
static value_string_ext DCallState_ext = VALUE_STRING_EXT_INIT(DCallState);

static const value_string CallPrivacy[] = {
  { 0x00000, "None" },
  { 0x00001, "Limited" },
  { 0x00002, "Full" },
  { 0x00000, NULL }
};
static value_string_ext CallPrivacy_ext = VALUE_STRING_EXT_INIT(CallPrivacy);

static const value_string DeviceUnregisterStatus[] = {
  { 0x00000, "Ok" },
  { 0x00001, "Error" },
  { 0x00002, "NAK" },
  { 0x00000, NULL }
};
static value_string_ext DeviceUnregisterStatus_ext = VALUE_STRING_EXT_INIT(DeviceUnregisterStatus);

static const value_string EndOfAnnAck[] = {
  { 0x00000, "NoAnnAckRequired" },
  { 0x00001, "AnnAckRequired" },
  { 0x00000, NULL }
};
static value_string_ext EndOfAnnAck_ext = VALUE_STRING_EXT_INIT(EndOfAnnAck);

static const value_string AnnPlayMode[] = {
  { 0x00000, "XmlConfigMode" },
  { 0x00001, "OneShotMode" },
  { 0x00002, "ContinuousMode" },
  { 0x00000, NULL }
};
static value_string_ext AnnPlayMode_ext = VALUE_STRING_EXT_INIT(AnnPlayMode);

static const value_string PlayAnnStatus[] = {
  { 0x00000, "OK" },
  { 0x00001, "Err" },
  { 0x00000, NULL }
};
static value_string_ext PlayAnnStatus_ext = VALUE_STRING_EXT_INIT(PlayAnnStatus);

#define MISCCOMMANDTYPE_VIDEOFREEZEPICTURE     0x00000
#define MISCCOMMANDTYPE_VIDEOFASTUPDATEPICTURE 0x00001
#define MISCCOMMANDTYPE_VIDEOFASTUPDATEGOB     0x00002
#define MISCCOMMANDTYPE_VIDEOFASTUPDATEMB      0x00003
#define MISCCOMMANDTYPE_LOSTPICTURE            0x00004
#define MISCCOMMANDTYPE_LOSTPARTIALPICTURE     0x00005
#define MISCCOMMANDTYPE_RECOVERYREFERENCEPICTURE 0x00006
#define MISCCOMMANDTYPE_TEMPORALSPATIALTRADEOFF 0x00007

static const value_string MiscCommandType[] = {
  { MISCCOMMANDTYPE_VIDEOFREEZEPICTURE, "videoFreezePicture" },
  { MISCCOMMANDTYPE_VIDEOFASTUPDATEPICTURE, "videoFastUpdatePicture" },
  { MISCCOMMANDTYPE_VIDEOFASTUPDATEGOB, "videoFastUpdateGOB" },
  { MISCCOMMANDTYPE_VIDEOFASTUPDATEMB, "videoFastUpdateMB" },
  { MISCCOMMANDTYPE_LOSTPICTURE, "lostPicture" },
  { MISCCOMMANDTYPE_LOSTPARTIALPICTURE, "lostPartialPicture" },
  { MISCCOMMANDTYPE_RECOVERYREFERENCEPICTURE, "recoveryReferencePicture" },
  { MISCCOMMANDTYPE_TEMPORALSPATIALTRADEOFF, "temporalSpatialTradeOff" },
  { 0x00000, NULL }
};
static value_string_ext MiscCommandType_ext = VALUE_STRING_EXT_INIT(MiscCommandType);

static const value_string MediaTransportType[] = {
  { 0x00001, "RTP" },
  { 0x00002, "UDP" },
  { 0x00003, "TCP" },
  { 0x00000, NULL }
};
static value_string_ext MediaTransportType_ext = VALUE_STRING_EXT_INIT(MediaTransportType);

static const value_string ResvStyle[] = {
  { 0x00001, "FF" },
  { 0x00002, "SE" },
  { 0x00003, "WF" },
  { 0x00000, NULL }
};
static value_string_ext ResvStyle_ext = VALUE_STRING_EXT_INIT(ResvStyle);

static const value_string SubscribeCause[] = {
  { 0x00000, "OK" },
  { 0x00001, "RouteFail" },
  { 0x00002, "AuthFail" },
  { 0x00003, "Timeout" },
  { 0x00004, "TrunkTerm" },
  { 0x00005, "TrunkForbidden" },
  { 0x00006, "Throttle" },
  { 0x00000, NULL }
};
static value_string_ext SubscribeCause_ext = VALUE_STRING_EXT_INIT(SubscribeCause);

static const value_string CallHistoryDisposition[] = {
  { 0x00000, "Ignore" },
  { 0x00001, "PlacedCalls" },
  { 0x00002, "ReceivedCalls" },
  { 0x00003, "MissedCalls" },
  { 0x0ffff, "UnknownDisp" },
  { 0x00000, NULL }
};
static value_string_ext CallHistoryDisposition_ext = VALUE_STRING_EXT_INIT(CallHistoryDisposition);

static const value_string MwiNotificationResult[] = {
  { 0x00000, "Ok" },
  { 0x00001, "GeneralError" },
  { 0x00002, "RequestRejected" },
  { 0x00003, "VmCountOutOfBounds" },
  { 0x00004, "FaxCountOutOfBounds" },
  { 0x00005, "InvalidPriorityVmCount" },
  { 0x00006, "InvalidPriorityFaxCount" },
  { 0x00000, NULL }
};
static value_string_ext MwiNotificationResult_ext = VALUE_STRING_EXT_INIT(MwiNotificationResult);

static const value_string RecordingStatus[] = {
  { 0x00000, "_OFF" },
  { 0x00001, "_ON" },
  { 0x00000, NULL }
};
static value_string_ext RecordingStatus_ext = VALUE_STRING_EXT_INIT(RecordingStatus);



static dissector_handle_t xml_handle;

/* Initialize the subtree pointers */
static gint ett_skinny          = -1;
static gint ett_skinny_tree     = -1;

/* preference globals */
static gboolean global_skinny_desegment = TRUE;

/* tap register id */
static int skinny_tap = -1;

/* skinny protocol tap info */
#define MAX_SKINNY_MESSAGES_IN_PACKET 10
static skinny_info_t pi_arr[MAX_SKINNY_MESSAGES_IN_PACKET];
static int pi_current = 0;
static skinny_info_t *si;

dissector_handle_t skinny_handle;



typedef void (*message_handler) (ptvcursor_t * cursor, packet_info *pinfo, skinny_conv_info_t * skinny_conv);

typedef struct _skinny_opcode_map_t {
  guint32 opcode;
  message_handler handler;
  skinny_message_type_t type;
  const char *name;
} skinny_opcode_map_t;

/* begin conversaton  info*/
typedef enum _skinny_message_type_t {
  SKINNY_MSGTYPE_EVENT    = 0,
  SKINNY_MSGTYPE_REQUEST  = 1,
  SKINNY_MSGTYPE_RESPONSE = 2,
} skinny_message_type_t;
  
/* Messages Handler Array */
static const skinny_opcode_map_t skinny_opcode_map[] = {
  {0x0000, handle_default_function                                           , SKINNY_MSGTYPE_REQUEST  , "KeepAliveReqMessage"},
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
  {0x0022, handle_default_function            , SKINNY_MSGTYPE_RESPONSE , "OpenReceiveChannelAckMessage"},
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
  {0x008a, handle_default_function           , SKINNY_MSGTYPE_REQUEST  , "StartMediaTransmissionMessage"},
  {0x008b, handle_default_function            , SKINNY_MSGTYPE_EVENT    , "StopMediaTransmissionMessage"},
  {0x008f, handle_default_function                         , SKINNY_MSGTYPE_EVENT    , "CallInfoMessage"},
  {0x0090, handle_default_function                   , SKINNY_MSGTYPE_RESPONSE , "ForwardStatResMessage"},
  {0x0091, handle_default_function                 , SKINNY_MSGTYPE_RESPONSE , "SpeedDialStatResMessage"},
  {0x0092, handle_default_function                      , SKINNY_MSGTYPE_RESPONSE , "LineStatResMessage"},
  {0x0093, handle_default_function                    , SKINNY_MSGTYPE_RESPONSE , "ConfigStatResMessage"},
  {0x0094, handle_default_function                      , SKINNY_MSGTYPE_RESPONSE , "TimeDateResMessage"},//---liudan
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
  {0x0111, handle_default_function                        , SKINNY_MSGTYPE_EVENT    , "CallStateMessage"},
  {0x0112, handle_default_function              , SKINNY_MSGTYPE_EVENT    , "DisplayPromptStatusMessage"},
  {0x0113, handle_default_function                , SKINNY_MSGTYPE_EVENT    , "ClearPromptStatusMessage"},
  {0x0114, handle_default_function                    , SKINNY_MSGTYPE_EVENT    , "DisplayNotifyMessage"},
  {0x0115, handle_default_function                                           , SKINNY_MSGTYPE_EVENT    , "ClearNotifyMessage"},
  {0x0116, handle_default_function                , SKINNY_MSGTYPE_EVENT    , "ActivateCallPlaneMessage"},
  {0x0117, handle_default_function                                           , SKINNY_MSGTYPE_EVENT    , "DeactivateCallPlaneMessage"},
  {0x0118, handle_default_function                    , SKINNY_MSGTYPE_RESPONSE , "UnregisterAckMessage"},
  {0x0119, handle_default_function                     , SKINNY_MSGTYPE_EVENT    , "BackSpaceResMessage"},
  {0x011a, handle_default_function                                           , SKINNY_MSGTYPE_RESPONSE , "RegisterTokenAck"},
  {0x011b, handle_default_function                     , SKINNY_MSGTYPE_RESPONSE , "RegisterTokenReject"},
  {0x011c, handle_default_function       , SKINNY_MSGTYPE_EVENT    , "StartMediaFailureDetectionMessage"},
  {0x011d, handle_default_function                     , SKINNY_MSGTYPE_EVENT    , "DialedNumberMessage"},
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
};


/*******************************************************************************************************/
	
#define skinny_log(fmt,...)  \
    _logger_file("/tmp/hzivy-skinny.log",__func__,__LINE__,fmt,##__VA_ARGS__);  


#define skinny_log_err(fmt,...)  \
						skinny_log("ERROR|"fmt,##__VA_ARGS__); 


struct sip_pkt
{
	int from_server;// 1  yes (this is respo pkt), 0 no. 0 is to server. this is request pkt.
	char* line;
	char*  sip_msg_body;

	struct sip_msg_header msg_hdr;
	u8 body_sdp;

	u16    rtp_port;//不知道是src dest.
	struct in_addr rtp_ip;
	//struct session_info* ss;

	enum session_state state;
};

static void sniffer_handle_skinny(u_char * user, const struct pcap_pkthdr * packet_header, const u_char * packet_content)
{
    int ret = 0;
    
	const struct pcap_pkthdr* phdr = packet_header;
	struct iphdr* iph = NULL;
	struct tcphdr* th = NULL;
	
	ret = check_iphdr(phdr,packet_content,&iph);
	if(ret != 0)
		goto error;
	
	if(0 != check_tcp(iph,&th))	
		goto error;
	
    //send_sip_pkt(iph,udph);/* 把sip报文转给upload一份。 */
    
	handler_skinny_element(th);
	
error:
	return;
}

int sniffer_loop_skinny( pcap_t *p)
{
	 pcap_loop( p,-1,sniffer_handle_skinny,(u_char*)NULL);
	 return 0;
}

/***********************************************
线程启动与执行体
************************************************/
/*
sniffer_sip_loop:
打开pcap_file,进入抓包执行体。

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
		sip_log_err("create  sniffer_skinny_loop failed\n");
		return -1;
	}

	return tid;

}



pthread_t sniffer_skinny_start(void)
{
	pthread_t tid;

	tid = __sniffer_skinny_start();
    sip_log("%s:%d tid %d\n",__func__,__LINE__,tid);
	return tid;

}

