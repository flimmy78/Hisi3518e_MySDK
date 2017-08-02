#ifndef _RTSP_H264_H_
#define _RTSP_H264_H_


#ifdef __cplusplus
extern "C" {
#endif 

#include "rtsp_struct.h"


#ifndef PACKED
#define PACKED	__attribute__((packed, aligned(1)))
#endif

//================================================================
//����:�ṹ��
//����:FUIndicatorDef
//����:FU���͵ڶ��ֽڵ�ָʾ
//================================================================
typedef struct tagFUIndicator
{
	char TYPE : 5;		/* set to 28 or 29 */
	char NRI : 2;		/* the same to nal */
	char F : 1; 		/* set to 0 */
}PACKED FUIndicatorDef;


/**************************************************************************
����:�ṹ��
����:FUHeaderDef
����:FU��һ�ֽ�ͷ�ṹ
***************************************************************************/
typedef struct tagFUHeader
{
	char Type : 5;		/* set to nal type */
	char R : 1; 		/* reserve */
	char E : 1; 		/* end flag */
	char S : 1; 		/* start flag */
}PACKED  FUHeaderDef;

/**************************************************************************
����:�ṹ��
����:FU_ADef
����:FUͷ�ṹ
***************************************************************************/
typedef struct tagFU_A
{
/*����RFC3984  FU-A��RTP���ظ�ʽ*/
/*+---------------+
  |0|1|2|3|4|5|6|7|
  +-+-+-+-+-+-+-+-+
  |F|NRI|Type = 1c|
  +---------------+*/
	FUIndicatorDef stFUIndicator;
/*+---------------+
  |0|1|2|3|4|5|6|7|
  +-+-+-+-+-+-+-+-+
  |S|E|R| NalType |
  +---------------+*/
	FUHeaderDef stFUHeader;
}PACKED FU_ADef;

int rtsp_h264_video_send(RTSP_SESSION_S* pSess, const char *szBuff, int nLen, unsigned long u32TimeStamp);

int rtsp_h264_get_media_info(char *pBuffData, int nFrameLen, char *proLevelId, char *szSpsPps);

#ifdef __cplusplus
}
#endif 

#endif
