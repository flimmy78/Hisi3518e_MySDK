#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

#include "debug.h"
#include "rtsp_parse.h"
#include "rtsp_global.h"
#include "rtsp_struct.h"


RTSP_TKN_S RTSP_Methods [] =
{
   {RTSP_METHOD_PLAY, RTSP_PLAY_METHOD},
   {RTSP_METHOD_PAUSE, RTSP_PAUSE_METHOD},
   {RTSP_METHOD_DESCRIBE, RTSP_DESCRIBE_METHOD},
   {RTSP_METHOD_SETUP, RTSP_SETUP_METHOD},
   {RTSP_METHOD_REDIRECT, RTSP_REDIRECT_METHOD},
   {RTSP_METHOD_SESSION, VOD_SESSION_METHOD},
   {RTSP_METHOD_OPTIONS, RTSP_OPTIONS_METHOD},
   {RTSP_METHOD_TEARDOWN, RTSP_TEARDOWN_METHOD},
   {RTSP_METHOD_RECORD, RTSP_RECORD_METHOD},
   {RTSP_METHOD_GET_PARAM, RTSP_GET_PARAM_METHOD},
   {RTSP_METHOD_SET_PARAM, RTSP_SET_PARAM_METHOD},
   {RTSP_METHOD_EXT_METHOD, RTSP_EXTENSION_METHOD},
   {0, RTSP_PARSE_INVALID_OPCODE}
};



RTSP_TKN_S RTSP_Status [] =
{
   {"Continue", 100},
   {"OK", 200},
   {"Created", 201},
   {"Accepted", 202},
   {"Non-Authoritative Information", 203},
   {"No Content", 204},
   {"Reset Content", 205},
   {"Partial Content", 206},
   {"Multiple Choices", 300},
   {"Moved Permanently", 301},
   {"Moved Temporarily", 302},
   {"Bad Request", 400},
   {"Unauthorized", 401},
   {"Payment Required", 402},
   {"Forbidden", 403},
   {"Not Found", 404},
   {"Method Not Allowed", 405},
   {"Not Acceptable", 406},
   {"Proxy Authentication Required", 407},
   {"Request Time-out", 408},
   {"Conflict", 409},
   {"Gone", 410},
   {"Length Required", 411},
   {"Precondition Failed", 412},
   {"Request Entity Too Large", 413},
   {"Request-URI Too Large", 414},
   {"Unsupported Media Type", 415},
   {"Bad Extension", 420},
   {"Invalid Parameter", 450},
   {"Parameter Not Understood", 451},
   {"Conference Not Found", 452},
   {"Not Enough Bandwidth", 453},
   {"Session Not Found", 454},
   {"Method Not Valid In This State", 455},
   {"Header Field Not Valid for Resource", 456},
   {"Invalid Range", 457},
   {"Parameter Is Read-Only", 458},
   {"Internal Server Error", 500},
   {"Not Implemented", 501},
   {"Bad Gateway", 502},
   {"Service Unavailable", 503},
   {"Gateway Time-out", 504},
   {"RTSP Version Not Supported", 505},
   {"Extended Error:", 911},
   {0, RTSP_PARSE_INVALID_OPCODE}
};

char  *RTSP_Invalid_Method_STR = "Invalid Method";

/* message header keywords */

char *rtsp_get_status_str( int code )
{
   RTSP_TKN_S  *pRtspTkn;
   
   for (pRtspTkn = RTSP_Status; pRtspTkn->opcode != RTSP_PARSE_INVALID_OPCODE; pRtspTkn++ )
   {
      if ( pRtspTkn->opcode == code )
      {
         return( pRtspTkn->token );
      }
   }

   return( RTSP_Invalid_Method_STR );
}


/***********************************************************************************
*Function Name   :rtsp_valid_resp_msg
*Description     :
*Input           :
*Output          :
*Return          :
				 :
*Other           :porting from rtsp_ref::valid_resp_msg
*Author          :hi
***********************************************************************************/
int rtsp_valid_resp_msg( char *pStr, unsigned short *pSeq_num, unsigned short *pStatus)

{
   char           ver [32];
   unsigned int   stat;
   unsigned int   seq;
   int            pcnt;          /* parameter count */

   char           uncare[256];

   /* assuming "stat" may not be zero (probably faulty) */
   stat = 0;
   seq = 0;

   pcnt = sscanf( pStr, " %31s %u  %255s ", ver, &stat,  uncare );


   if ( strncasecmp ( ver, "RTSP/", 5 ) )
   {
   	  LOG_WARN("RTSP_PARSE_ISNOT_RESP\n");
      return( RTSP_PARSE_ISNOT_RESP);  /* not a response message */
   }

   /* confirm that at least the version, status code and sequence are there. */
   if ( pcnt < 3 || stat == 0 )
   {
      return( RTSP_PARSE_ISNOT_RESP );  /* not a response message */
   }

   *pStatus = stat;
   return(RTSP_PARSE_IS_RESP);
}


/***********************************************************************************
*Function Name   :rtsp_get_cseq
*Description     :
*Input           :
*Output          :
*Return          :
				 :
*Other           :
*Author          :hi
***********************************************************************************/
int rtsp_get_cseq(char *pStr)
{
    int cseq = -1;
    char * pTemp = NULL;
    /*��ȡCSeq��Ϣ*/
	if ((pTemp = strstr(pStr, RTSP_HDR_CSEQ)) == NULL) 
    {
        LOG_WARN("Not found 'CSeq'.\n");
	} 
    else 
    {
        char trash[255] = {0};
		if (sscanf(pTemp, "%254s %d", trash, &(cseq)) != 2) 
        {
            LOG_WARN("[%s]Not found 'CSeq' number.\n", pTemp);
		}
	}
    return cseq;
}


/***********************************************************************************
*Function Name   :rtsp_valid_req
*Description     :
*Input           :
*Output          :
*Return          :
				 :
*Other           :
*Author          :hi
***********************************************************************************/
int rtsp_valid_req( char *pStr )
{
    RTSP_TKN_S     *m;
    char           method [32];
    char           object [256];
    char           ver [32];
    int            pcnt;          /* parameter count */
    int            seq;
    /*
    * Check for a valid method token and convert
    * it to a switch code.
    * Returns -2 if there are not 4 valid tokens in the request line.
    * Returns -1 if the Method token is invalid.
    * Otherwise, the method code is returned.
    */
    *method = *object = '\0';
    seq = 0;

    pcnt = sscanf( pStr, " %31s %255s %31s", method, object, ver );

    if ( pcnt != 3 )
    {
        LOG_ERROR("\n*************\n%s\n*************is Not a valid req msg.\n", pStr);
    }

    for ( m = RTSP_Methods ; m->opcode != -1; m++ )
    {
        if ( !strcmp( m->token, method ) )
        {
            break;
        }

    }

    return( m->opcode );
}


/* expects format 'rtsp://server[:port]/filename' */
int rtsp_parse_url(const char *url, char *server, int *port, char *file_name)
{
	int ret = -1;
	unsigned short i = 0;
	char *token, *port_str, *end_ptr, *url_l;
	char *full = NULL;
	int has_port = 0, has_url = 0, len = 0, url_len = 0;
	
	if (url == NULL)
		return -1;
	
    full = calloc(1, strlen(url) + 1);
    *port = RTSP_DEFAULT_SVR_PORT;
	
	strcpy(full, url);
	if (strncmp(full, "rtsp://", 7) == 0) 
    {
		char *aSubStr = malloc(strlen(url) + 1);
		strcpy(aSubStr, &full[7]);
		len = strcspn(aSubStr, " /\t\n");
		for (; (i < strlen(url)); i++)
		{
			aSubStr[i] = 0;
		}
		strncpy(aSubStr, &full[7], len);
		if((NULL != strtok(aSubStr,"@")) && (url_l = strtok(NULL,"@")))
		{
			strncpy(aSubStr, url_l, strlen(url));
			if ((end_ptr = strchr(aSubStr, ':')) != NULL)
			{
				url_len = end_ptr - aSubStr;
				strncpy(server, url_l, url_len);
				end_ptr = end_ptr + 1;
				port_str = strtok(end_ptr, " /\t\n");
				if (port_str)
				{
					*port = atol(port_str);//port=554
				}
				has_port = 1;       //���ڶ˿ں�	
			}
			else
			{
				strncpy(server, url_l, strlen(server));
				
			}
			ret = 0;
			token = strtok(&full[7 + len + 1], " ");
			if (token)
			    strcpy(file_name, token);
			else
			    file_name[0] = '\0';
			has_url = 1;            //url���ڱ�־		
		
		}
		if (has_port == 0 && strchr(aSubStr, ':'))
		{
			has_port = 1;           //���ڶ˿ں�
		}
		free(aSubStr);
        /*TODO ?*/
		token = strtok(&full[7], " :/\t\n");
		if ((!has_url) && token) 
       	{
			strcpy(server, token);          //server=192.168.86.252
			if (has_port)
			{
				port_str = strtok(&full[strlen(server) + 7 + 1], " /\t\n");
				if (port_str)
				{
					*port = atol(port_str); //port=554
				}
			} 
			/* don't require a file name */
			ret = 0;
			token = strtok(NULL, " ");
			if (token)
				strcpy(file_name, token);
			else
				file_name[0] = '\0';
		}
	} 
	else 
	{
		/* try just to extract a file name */
		token = strtok(full, " \t\n");
		if (token) 
        {
			strncpy(file_name, token,16);
			server[0] = '\0';
			ret = 0;
		}
	}
	free(full);
    
	return ret;
}

/***********************************************************************************
*Function Name   	:RTSP_prase_userinfo
*Description     	:�����û���֤��Ϣ����
*Input           		:
*Output          		:
*Return          		:
*Other           		:
*Author          		:hi
***********************************************************************************/
int rtsp_prase_user_info(char *buff,char *name,char *passwd,char *uri)
{
	char *p,*q,*r = NULL;
	char *outer_ptr = NULL;
	char *inner_ptr = NULL;
	p = strstr(buff, RTSP_HDR_AUTHORIZATION);
	p = p + 22;//p ָ��username
	r = p;

	while ((p = strtok_r(p, ",", &outer_ptr)) != NULL)
	{
		q = strtok_r(p, "=", &inner_ptr);
		if(0 == strcmp(q,"username"))
		{
			if(NULL != (q = strtok_r(NULL,"=",&inner_ptr)))
			{
				q = q+1;
				strncpy(name,q, RTSP_MAX_USERNAME_LEN-1);
				name[RTSP_MAX_USERNAME_LEN-1] = 0;
				q = strrchr(name,'"');
				if(q)
					*q = 0;
			}
			else
			{
				memset(name,0, RTSP_MAX_USERNAME_LEN);
				return -1;
			}
			p = NULL;
		}
		else if(0 == strcmp(q+1,"uri"))
		{
			if(NULL != (q = strtok_r(NULL,"=",&inner_ptr)))
			{
				q = q+1;
				strncpy(uri,q,127);
				uri[127-1] = 0;
				q = strrchr(uri,'"');
				if(q)
					*q = 0;
			}
			else
			{
		//		memset(uri,0,sizeof(uri));
		        memset(uri, 0, strlen(uri));
				return -1;
			}
			p = NULL;
		}
		else if(0 == strcmp(q+1,"response"))//�ڷָ�ʱ responseǰ��һ���ո�
		{
			if(NULL != (q = strtok_r(NULL,"=",&inner_ptr)))
			{
				q = q+1;
				strncpy(passwd, q, RTSP_MAX_USERPASSWD_LEN-1);
				passwd[RTSP_MAX_USERPASSWD_LEN-1]=0;
				q = strrchr(passwd,'"');
				if(q)
					*q = 0;
			}
			else
			{
				memset(passwd,0,RTSP_MAX_USERPASSWD_LEN);
				return -1;
			}
			p = NULL;
		}
		p = NULL;
	}
	
	
	return 0;
	
}

/***********************************************************************************
*Function Name   :rtsp_full_msg_parse()
*Description     :RTSP��Ϣ�Ϸ��Է���
*Input           :
*Output          :
*Return          :
*Athor           :hi
*Other           : porting for fenice1.11 RTSP_full_msg_rcvd.c
				 * \return -1 on ERROR
				 * \return RTSP_not_full (0) if a full RTSP message is NOT present in the in_buffer yet.
				 * \return RTSP_method_rcvd (1) if a full RTSP message is present in the in_buffer and is
				 * ready to be handled.
				 * \return RTSP_interlvd_rcvd (2) if a complete RTP/RTCP interleaved packet is present.  
				 * terminate on really ugly cases.
*/
int rtsp_full_msg_parse(const RTSP_SESSION_S *pSess, int *hdr_len, int *body_len)
// This routine is from OMS.
{
	int eomh;		/* end of message header found */
	int mb;			/* message body exists */
	int tc;			/* terminator count */
	int ws;			/* white space */
	unsigned int ml;			/* total message length including any message body */
	int bl;			/* message body length */
	char c;			/* character */
	int control;
	char *p;

	// is there an interleaved RTP/RTCP packet?
	if (pSess->szBuffRecv[0] == '$') {
		unsigned short *intlvd_len = (unsigned short *)&pSess->szBuffRecv[2];
		
		if ( (bl = ntohs(*intlvd_len)) <= pSess->nMsgSize) {
			// fnc_log(FNC_LOG_DEBUG,"Interleaved RTP or RTCP packet arrived (len: %hu).\n", bl);
			if (hdr_len)
				*hdr_len = 4;
			if (body_len)
				*body_len = bl;
			return RTSP_INTERLVD_RCVD;
		} else {
//			LOG_WARN("Non-complete Interleaved RTP or RTCP packet arrived\n");
			return RTSP_NOT_FULL;
		}
		
	}
	eomh = mb = ml = bl = 0;
	while (ml <= pSess->nMsgSize) {
		/* look for eol. */
		control = strcspn(&(pSess->szBuffRecv[ml]), "\r\n");
		if(control > 0)
			ml += control;
		else
			return(-1);
		
		if (ml > pSess->nMsgSize)
			return RTSP_NOT_FULL;	/* haven't received the entire message yet. */
		/*
		 * work through terminaters and then check if it is the
		 * end of the message header.
		 */
		tc = ws = 0;
		while (!eomh && ((ml + tc + ws) < pSess->nMsgSize)) {
			c = pSess->szBuffRecv[ml + tc + ws];
			if (c == '\r' || c == '\n')
				tc++;
			else if ((tc < 3) && ((c == ' ') || (c == '\t')))
				ws++;	/* white space between lf & cr is sloppy, but tolerated. */
			else
				break;
		}
		/*
		 * cr,lf pair only counts as one end of line terminator.
		 * Double line feeds are tolerated as end marker to the message header
		 * section of the message.  This is in keeping with RFC 2068,
		 * section 19.3 Tolerant Applications.
		 * Otherwise, CRLF is the legal end-of-line marker for all HTTP/1.1
		 * protocol compatible message elements.
		 */
		if ((tc > 2) || ((tc == 2) && (pSess->szBuffRecv[ml] == pSess->szBuffRecv[ml + 1])))
			eomh = 1;	/* must be the end of the message header */
		ml += tc + ws;

		if (eomh) {
			ml += bl;	/* add in the message body length, if collected earlier */
			if (ml <= pSess->nMsgSize)
				break;	/* all done finding the end of the message. */
		}

		if (ml >= pSess->nMsgSize)
			return RTSP_NOT_FULL;	/* haven't received the entire message yet. */

		/*
		 * check first token in each line to determine if there is
		 * a message body.
		 */
		if (!mb) {	/* content length token not yet encountered. */
			if (!strncasecmp(&(pSess->szBuffRecv[ml]), RTSP_HDR_CONTENTLENGTH, strlen(RTSP_HDR_CONTENTLENGTH))) {
				mb = 1;	/* there is a message body. */
				ml += strlen(RTSP_HDR_CONTENTLENGTH);
				while (ml < pSess->nMsgSize) {
					c = pSess->szBuffRecv[ml];
					if ((c == ':') || (c == ' '))
						ml++;
					else
						break;
				}

				if (sscanf(&(pSess->szBuffRecv[ml]), "%d", &bl) != 1) {
					return (-1);
				}
			}
		}
	}
	
	if (hdr_len)
		*hdr_len = ml - bl;

	if (body_len) {		
		/*
		 * go through any trailing nulls.  Some servers send null terminated strings
		 * following the body part of the message.  It is probably not strictly
		 * legal when the null byte is not included in the Content-Length count.
		 * However, it is tolerated here.
		 */
		for (tc = pSess->nMsgSize - ml, p = (char *)&(pSess->szBuffRecv[ml]); tc && (*p == '\0'); p++, bl++, tc--);
		*body_len = bl;
	}
	
	return RTSP_METHOD_RCVD;
}

