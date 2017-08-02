#ifndef _HI_RTSP_STRUCT_H_
#define _HI_RTSP_STRUCT_H_

#include   <netinet/in.h> 
#include   <arpa/inet.h> 
#include   <sys/types.h> 
#include   <sys/socket.h> 

#include "linux_list.h"

#ifdef __cplusplus
extern "C" {
#endif 

#define RTSP_MAX_CAMERA         3
#define RTSP_MAX_STREAM	        6   //
#define RTSP_MAX_STREAM_TYPE    2   //0  video 1 audio
#define RTSP_MAX_CLIENT			16  //���������
#define DEFAULT_RTSP_PORT       554

#define RTSP_MAX_FRAME_LEN		1024*1024
/*rtsp��socket�շ�buffer��С*/
#define RTSP_MAX_PROTOCOL_BUFFER 4096
#define RTP_MAX_PACKET_BUFF 	 1460
#define RTSP_DEFAULT_SVR_PORT	 554
#define RTSP_MAX_USERNAME_LEN	 32
#define RTSP_MAX_USERPASSWD_LEN	 32
#define RTP_DEFAULT_SSRC 		 14080
//========================================================
#define RTSP_VER_STR  "RTSP/1.0"

#define RTSP_LR   "\r"
#define RTSP_LF   "\n"
#define RTSP_LRLF "\r\n"

#define RTSP_METHOD_SETUP      "SETUP"
#define RTSP_METHOD_REDIRECT   "REDIRECT"
#define RTSP_METHOD_PLAY       "PLAY"
#define RTSP_METHOD_PAUSE      "PAUSE"
#define RTSP_METHOD_SESSION    "SESSION"
#define RTSP_METHOD_RECORD     "RECORD"

#define RTSP_METHOD_EXT_METHOD "EXT-"

#define RTSP_METHOD_OPTIONS    "OPTIONS"
#define RTSP_METHOD_DESCRIBE   "DESCRIBE"
#define RTSP_METHOD_GET_PARAM  "GET_PARAMETER"
#define RTSP_METHOD_SET_PARAM  "SET_PARAMETER"
#define RTSP_METHOD_TEARDOWN   "TEARDOWN"

/* message header keywords */
#define RTSP_HDR_CONTENTLENGTH "Content-Length"
#define RTSP_HDR_ACCEPT "Accept"
#define RTSP_HDR_ALLOW "Allow"
#define RTSP_HDR_BLOCKSIZE "Blocksize"
#define RTSP_HDR_CONTENTTYPE "Content-Type"
#define RTSP_HDR_DATE "Date"
#define RTSP_HDR_REQUIRE "Require"
#define RTSP_HDR_TRANSPORTREQUIRE "Transport-Require"
#define RTSP_HDR_SEQUENCENO "SequenceNo"
#define RTSP_HDR_CSEQ "CSeq"
#define RTSP_HDR_STREAM "Stream"
#define RTSP_HDR_SESSION "Session"
#define RTSP_HDR_TRANSPORT "Transport"
#define RTSP_HDR_RANGE "Range"	
#define RTSP_HDR_USER_AGENT "User-Agent"	
#define RTSP_HDR_AUTHORIZATION "Authorization"

typedef enum hiRTSP_REQ_METHOD
{
    /* method codes */
    RTSP_SETUP_METHOD     = 0 ,
    RTSP_DESCRIBE_METHOD  = 1 ,
    RTSP_REDIRECT_METHOD  = 2 ,
    RTSP_PLAY_METHOD      = 3 ,
    RTSP_PAUSE_METHOD     = 4 ,
    VOD_SESSION_METHOD    = 5 ,
    RTSP_OPTIONS_METHOD   = 6 ,
    RTSP_RECORD_METHOD    = 7 ,
    RTSP_TEARDOWN_METHOD  = 8 ,
    RTSP_GET_PARAM_METHOD = 9 ,
    RTSP_SET_PARAM_METHOD = 10,
    RTSP_EXTENSION_METHOD = 11,
    RTSP_REQ_METHOD_BUTT 
}RTSP_REQ_METHOD_E;

/*
 * method response codes.  These are 100 greater than their
 * associated method values.  This allows for simplified
 * creation of event codes that get used in event_handler()
 */
#define RTSP_MAKE_RESP_CMD(req) (req + 100) 
typedef enum _RTSP_REP_
{
    RTSP_SETUP_RESPONSE      = RTSP_MAKE_RESP_CMD(RTSP_SETUP_METHOD),
    RTSP_GET_RESPONSE        ,/*101*/
    RTSP_REDIRECT_RESPONSE   ,/*102*/
    RTSP_PLAY_RESPONSE       ,/*103*/
    RTSP_PAUSE_RESPONSE      ,/*104*/
    VOD_SESSION_RESPONSE    ,/*105*/
    RTSP_OPTIONS_RESPONSE    ,/*106*/
    RTSP_RECORD_RESPONSE     ,/*107*/
    RTSP_TEARDOWN_RESPONSE   ,/*108*/
    RTSP_GET_PARAM_RESPONSE  ,/*109*/
    RTSP_SET_PARAM_RESPONSE  ,/*110*/
    RTSP_EXTENSION_RESPONSE  ,/*111*/
    RTSP_RESP_BUTT
}RTSP_RESP_E;

/* user command event codes */
#define RTSP_CMD_OPEN		10000
#define RTSP_CMD_GET		10001
#define RTSP_CMD_PLAY		10002
#define RTSP_CMD_PAUSE		10003
#define RTSP_CMD_CLOSE		10004


#define RTSP_STATUS_CONTINUE            100
#define RTSP_STATUS_OK                  200
#define RTSP_STATUS_ACCEPTED            202
#define RTSP_STATUS_BAD_REQUEST         400
#define RTSP_STATUS_METHOD_NOT_ALLOWED  405

typedef struct RTSP_TOKENS_
{
   char  *token;
   int   opcode;
} RTSP_TKN_S;

/*RTP Payload type define*/
typedef enum _RTP_PT_E_
{
    RTP_PT_ULAW             = 0,        /* mu-law  PCMU*/
    RTP_PT_GSM              = 3,        /* GSM */
    RTP_PT_G723             = 4,        /* G.723 */
    RTP_PT_ALAW             = 8,        /* a-law  PCMA*/
    RTP_PT_G722             = 9,        /* G.722 */
    RTP_PT_S16BE_STEREO     = 10,       /* linear 16, 44.1khz, 2 channel */
    RTP_PT_S16BE_MONO       = 11,       /* linear 16, 44.1khz, 1 channel */
    RTP_PT_MPEGAUDIO        = 14,       /* mpeg audio */
    RTP_PT_JPEG             = 26,       /* jpeg */
    RTP_PT_H261             = 31,       /* h.261 */
    RTP_PT_MPEGVIDEO        = 32,       /* mpeg video */
    RTP_PT_MPEG2TS          = 33,       /* mpeg2 TS stream */
    RTP_PT_H263             = 34,       /* old H263 encapsulation */
                             
    RTP_PT_H264             = 96,       /* */
    RTP_PT_G726             = 97,       /* */
	RTP_PT_AAC              = 96, 
	RTP_PT_ADPCM			= 99,
	
    RTP_PT_INVALID          = 127
}RTP_PT_E;

typedef enum _AV_TYPE_E
{
    AV_TYPE_VIDEO = 0,
    AV_TYPE_AUDIO,
    AV_TYPE_AV,
    AV_TYPE_BUTT
} AV_TYPE_E;

/*RTP ����ģʽ*/
typedef enum _RTP_TRANSPORT_TYPE_E_
{
	RTP_TRANSPORT_UDP = 0,
	RTP_TRANSPORT_TCP,
	RTP_TRANSPORT_BUTT
}RTP_TRANSPORT_TYPE_E;

typedef struct _RTP_STATS_S
{
    unsigned long long  u64SendPacket;    /* number of packets send */
    unsigned long long  u64SendBytes;      /* bytes sent */
    unsigned long long  u64SendError;     /* error times when send */
    unsigned long long  u64RecvPacket;    /* number of packets received */
    unsigned long long  u64RecvBytes;      /* bytes of payload received */
    unsigned long long  u64Unavaillable;   /* packets not availlable when they were queried */
    unsigned long long  u64Bad;            /* packets that did not appear to be RTP */
    unsigned long long  u64Discarded;      /* incoming packets discarded because the queue exceeds its max size */
    unsigned long long  u64TimeoutCnt;
} RTP_STATS_S;

/*RTP TCP �����ǩ*/
typedef struct {
	int RTP;
	int RTCP;
} RTP_TCP_TOKEN;

/*RTSP״̬������ʱû��ʹ��*/
typedef enum RTSP_SESSION_STATE
{
    RTSP_STATE_INIT        = 0,
    RTSP_STATE_READY       = 1,
    RTSP_STATE_PLAY        = 2,
    RTSP_STATE_STOP        = 3,
    RTSP_STATE_BUTT  
}RTSP_SESSION_STATE_E;

/*rtp��������������*/
#define RTSP_STREAM_VIDEO 0
#define RTSP_STREAM_AUDIO 1
#define RTSP_STREAM_MAX   2

typedef struct _RTP_SENDER_S
{
	RTP_TCP_TOKEN		stInterleaved[2];     /*interleaved for TCP video and audio*/
	unsigned int 			nAudioSsrc;
    unsigned int 			nVideoSsrc;
	unsigned int            	nPayLoad[2];
    unsigned short 			u16LastSn[2];			/*last recv sn*/
    unsigned short 			u16LastTs[2];			/*last recv sn*/
	unsigned short          	u16LastSndSeq[2];
    unsigned short  			u16ChnId;				/*��sender����Ӧ��ͨ����*/
	int                     		nRtpTcpSock;
	int                     		nRtpSock[RTSP_MAX_STREAM_TYPE];				/*send/recv socket*/
	int                     		nRtcpSock[RTSP_MAX_STREAM_TYPE];
	int                     		nRtpPort[RTSP_MAX_STREAM_TYPE];
	int                     		nRtcpPort[RTSP_MAX_STREAM_TYPE];
	struct sockaddr_in       addrRemote[2];
	unsigned long           	u32LastSndPts[2];			/*��һ֡���ݵ�pts*/
	unsigned long           	u32LastSndTs[2];          /*��һ֡���ݵ�timestamp*/
	char           		   	*pFrameBuf;
    unsigned char 			szBuff[RTP_MAX_PACKET_BUFF];	/*������������ݵ�rtp buffer, ����Ѿ���rtp���������*/
    unsigned int 			nBuffLen;			/*buff�е�ʵ����Ҫ�������ݵĳ���*/ 
}RTP_SENDER_S;

//struct hiRTP_TARGETHOST;
typedef struct _RTSP_SESSION_S_ 
{       
    struct list_head	 list;						        /*����ά������*/
    int                  nCameraId;                         /* ��Ӧ��ͬ��camera */
    int					 nChId;								/*��Session����Ӧ��streamͨ����[0 ������]*/
	RTP_TRANSPORT_TYPE_E eTransType;							/*Transport: RTP/AVP/TCP  Transport: RTP/AVP*/
	RTP_TCP_TOKEN		stInterleaved[RTSP_MAX_STREAM_TYPE];	/*interleaved for TCP video & TCP audio*/
    char				szId[16];							/*��session����Ӧ��session id, �����Ӵ���ʱ���������*/
    char				szSSRC[16];
    char				szUserAgent[128];
    char				szUri[256];							/*��session�У� �û������url�� rtsp://xxxxxx:553/1 */
    int					nRtspSocket;						/*��session�Ķ�Ӧsocket�������Ӵ���ʱ�򱻸�ֵ*/
	char				szHostIP[64];							/*����ip*/
    char				szRemoteIP[64];						/*�ͻ��˵�ip��ַ*/
    char				szBuffRecv[RTSP_MAX_PROTOCOL_BUFFER]; /*rtsp������Ϣ��buffer*/
    char				szBuffSend[RTSP_MAX_PROTOCOL_BUFFER]; /*rtsp������Ϣ��buffer*/
	int					nMsgSize;							/*rtsp������Ϣ����*/
    unsigned short		u16RemotePort;							/*�ͻ��˵�rtsp���Ӷ˿�*/
	unsigned char		u8RequestStreamFlag[RTSP_MAX_STREAM_TYPE];		/*�Ƿ�������Ƶ�� ��Ƶ�ı�־*/
	unsigned short		u16RemoteRTPPort[RTSP_MAX_STREAM_TYPE];	/*�ͻ���rtp�˿� [0] ����Ƶ�˿ڣ� 1����Ƶ�˿�*/
    unsigned short		u16RemoteRTCPPort[RTSP_MAX_STREAM_TYPE];/*�ͻ���rtcp�˿� [0] ����Ƶ�˿ڣ� 1����Ƶ�˿�*/  
    unsigned long 		u32LastSndReq;						/*����͵�Req*/
    unsigned long		u32LastSndSeq;						/*����͵�Cseq*/
    unsigned long		u32LastRecvSeq;						/*�����������Cseq*/
	int					nLan;								/*����ip����ͬһ����������*/
	int					nLink;							/*��ʾ�Ƿ��Ѿ�����link��Ϣ�ص�*/
	RTSP_SESSION_STATE_E eState;
	RTP_SENDER_S	    stRtpSender;	/* tcp/udp ����*/
	pthread_t			sessThd;
	char 				nonce[17];
	pthread_mutex_t	mutex;
	time_t 			preTime;
	struct list_head	 transList;
}RTSP_SESSION_S;

typedef enum _RTSP_SVR_STATE_E
{
    VOD_SVR_STATE_INIT    = 0, /*��ʼ��״̬�� ��û�б�����*/
    VOD_SVR_STATE_RUNNING = 1, /*�Ѿ�����������������*/
    VOD_SVR_STATE_STOP    = 2, /*�Ѿ���������ֹͣû������*/
    VOD_SVR_STATE_BUTT
}RTSP_SVR_STATE_E;

typedef struct _RTSP_SVR
{
	pthread_t thd;                              /* �����������߳� */                        
	int 	  nSvrSocket;       	            /* rtsp socket */
	int   	  nSvrPort;                         /* rtsp �˿� */
	int       nUseAuth;                         /* �Ƿ����֤ */
	int	      nSvrExits;                        /* ����ֹͣ */
	RTSP_SVR_STATE_E  eState;                   /* ״̬�� */
	struct list_head  sessList;                        /* �Ự����ͷ */
	unsigned long   clientCnt[RTSP_MAX_STREAM];
}RTSP_SVR;

/* ����ȫ�ֱ��� */
extern RTSP_SVR	            gRtspSvr;
extern pthread_mutex_t		gRtspMutex;
/* end ����ȫ�ֱ��� */

#ifdef __cplusplus
}
#endif

#endif
