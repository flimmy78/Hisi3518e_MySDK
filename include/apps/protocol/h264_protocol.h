#ifndef     __H264_PROCOTOL_H__
#define     __H264_PROCOTOL_H__

#ifdef		__cplusplus
extern 	"C"	{
#endif

#include "common.h"

#define			CTRL_START			    "CTRL_START"
#define			STREAM_START		    "STREAM_START"
#define			TALK_START				"TALK_START"

#define     ALIVE_TIME              (60)        /* 60s */
#define     MAJOR_VERSION           (1)         /* ���汾��(�ش��޸�+1) */
#define     MINOR_VERSION           (2)         /* �Ӱ汾��(��ӹ���ʱ+1) */
#define     REVISION_VERSION         (1)         /* �����汾��(�޸�bugʱ+1) */

/* ������� */
typedef enum H264_CTRL_VALUE_T
{
	CMD_LOGIN_IN = 0,
	CMD_LOGIN_OUT,
	CMD_START_STREAM,
	CMD_STOP_STREAM,
	CMD_DESTORY_STREAM,
	CMD_START_SPEAKER,
	CMD_STOP_SPEAKER,
	CMD_GET_PARAM,
	CMD_SET_PARAM,
	CMD_GET_S_PARAM,
	CMD_SET_S_PARAM,	
	CMD_IPC_EVENT,
	CMD_KEEP_ALIVE,
	CMD_SEARCH_RECLIST,
	CMD_GET_FRAME,
	CMD_GET_PRE_VIDEO,
	CMD_CREATE_TALK,
	CMD_CREATE_STREAM,	
	CMD_CREATE_CTRL,	
	CMD_GET_PARAM_EXT,
	CMD_SET_PARAM_EXT,	
	CMD_PROC_EXT,
	CMD_GET_DEVINFO,
	CMD_WIFI_TEST,          /* wifi ���� */	
	CMD_MAX_INDEX,
}H264_CTRL_VALUE;

/* ��������ͷ */
typedef struct DATA_HEAD_T
{
	char mark[4];				/* �ݶ��� "TNVS" ����У�������Ƿ���ȷ */
	int type;
	int len;					
    int index;                /* ���ͺͽ����������,��֤���ͺͽ��յ�����ͬ�� */
}DATA_HEAD;

/*************************   �ͻ��˷��ͳ���������   **************************/
typedef struct PC_LOGIN_IN_T
{
    char user[STR_64_LEN];
    char pwd[STR_64_LEN];
}PC_LOGIN_IN;  

typedef struct PC_LOGIN_OUT_T
{
    char user[STR_64_LEN];
    char pwd[STR_64_LEN];
}PC_LOGIN_OUT;  

typedef struct PC_CREATE_CTRL_T
{
    int result;
}PC_CREATE_CTRL;

#if 0
typedef struct PC_CREATE_STREAM_T
{
    int stream_id;
    int result;
}PC_CREATE_STREAM;
#endif

typedef struct PC_CREATE_TALK_T
{
    int result;
}PC_CREATE_TALK;

typedef struct PC_START_STREAM_T
{
    int stream_id;
    int stream_mask;
}PC_START_STREAM;

typedef struct PC_STOP_STREAM_T
{
    int stream_id;
    int stream_mask;
}PC_STOP_STREAM;

typedef struct PC_START_SPEAKER_T
{
    int stream_id;
}PC_START_SPEAKER;

typedef struct PC_STOP_SPEAKER_T
{
    int stream_id;
}PC_STOP_SPEAKER;

typedef struct PC_GET_PARAM_T	/* ��ȡ������ʱ���Բ����ṹ��Ϊ��λ */
{
	int type;	
}PC_GET_PARAM;

typedef struct PC_SET_PARAM_T	/* ���ò�����ֻ�ܵ�����������(����ǿ�������ṹ������,������������������) */
{
	int type;
    int len;
    char data[STR_512_LEN];            
}PC_SET_PARAM;

typedef struct PC_GET_S_PARAM_T
{
	char name[STR_64_LEN];	
}PC_GET_S_PARAM;

typedef struct PC_SET_S_PARAM_T
{
    char name[STR_64_LEN];
    char data[STR_64_LEN];            
}PC_SET_S_PARAM;

typedef struct PC_SEARCH_RECLIST_T
{
	int type;	
    char start_time[STR_64_LEN];
    char end_time[STR_64_LEN];    
}PC_SEARCH_RECLIST;

typedef struct PC_KEEP_ALIVE_T
{
    int result;
}PC_KEEP_ALIVE;

typedef struct PC_DESTORY_STREAM_T
{
    int stream_id;
}PC_DESTORY_STREAM;


/*******************************    �豸���ͳ���������    *****************************/
typedef struct IPC_LOGIN_IN_T
{
	int result;
    int level;
	int session_id;
}IPC_LOGIN_IN;

typedef struct IPC_LOGIN_OUT_T
{
    int result; 
}IPC_LOGIN_OUT;

typedef struct IPC_CREATE_CTRL_T
{
    int result;
}IPC_CREATE_CTRL;

typedef struct IPC_CREATE_STREAM_T
{
    int stream_id;
    int result;
}IPC_CREATE_STREAM;

typedef struct IPC_CREATE_TALK_T
{
    int result;
}IPC_CREATE_TALK;

typedef struct IPC_START_STREAM_T
{
    int result;
}IPC_START_STREAM;

typedef struct IPC_STOP_STREAM_T
{
    int result;
}IPC_STOP_STREAM;

typedef struct IPC_START_SPEAKER_T
{
    int result;
}IPC_START_SPEAKER;

typedef struct IPC_STOP_SPEAKER_T
{
    int result;
}IPC_STOP_SPEAKER;

typedef struct IPC_GET_PARAM_T
{
    int result;
    int len;
	int data[STR_512_LEN];
}IPC_GET_PARAM;

typedef struct IPC_SET_PARAM_T
{
    int result; 
}IPC_SET_PARAM;

typedef struct IPC_GET_S_PARAM_T
{
	int result;
	char data[STR_64_LEN];	
}IPC_GET_S_PARAM;

typedef struct IPC_SET_S_PARAM_T
{
    int result; 
}IPC_SET_S_PARAM;

typedef struct IPC_SEARCH_RECLIST_T
{
    int result;
	int len;	
	char *data;	
}IPC_SEARCH_RECLIST;

typedef struct IPC_KEEP_ALIVE_T
{
    int result;
}IPC_KEEP_ALIVE;

typedef struct IPC_EVENT_T
{
	int type;
	char data[STR_512_LEN];
}IPC_EVENT;

typedef struct IPC_DESTORY_STREAM_T
{
    int result;
}IPC_DESTORY_STREAM;


int CheckH264Procotol(int fd, char *http_request);

#ifdef		__cplusplus
}
#endif

#endif

