#ifndef _RTSP_PARSE_H_
#define _RTSP_PARSE_H_


#include "rtsp_struct.h"

#ifdef __cplusplus
extern "C" {
#endif 


/*
* Copyright (c) 2010
* All rights reserved.
* 
* filename:hi_rtsp_parse.h
* 
* func:���ļ�����ʵ���˶�rtsp�Ự����
* modify list: 
* 
*/
// RTSP parsing helper functions
#define RTSP_NOT_FULL		0
#define RTSP_METHOD_RCVD	1
#define RTSP_INTERLVD_RCVD  2


/*�ж��Ƿ���resp��Ϣ�� ����RSTP_Valid_RespMsg*/
#define RTSP_PARSE_OK 			   0

#define RTSP_PARSE_INVALID_OPCODE -1

#define RTSP_PARSE_INVALID        -2

#define RTSP_PARSE_ISNOT_RESP     -3

#define RTSP_PARSE_IS_RESP        -4

int rtsp_full_msg_parse(const RTSP_SESSION_S *pSess, int *hdr_len, int *body_len);

int rtsp_valid_resp_msg(char *pStr, unsigned short *pSeq_num, unsigned short *pStatus);

int rtsp_parse_url(const char *url, char *server, int *port, char *file_name);

int rtsp_prase_user_info(char *buff,char *name,char *passwd,char *uri);

int rtsp_valid_req(char *pStr );

int rtsp_get_cseq(char *pStr);

char *rtsp_get_status_str( int code );

#ifdef __cplusplus
}
#endif 

#endif

