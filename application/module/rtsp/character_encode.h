#ifndef _CHARACTER_ENCODE_H
#define _CHARACTER_ENCODE_H

/*
* Copyright (c) 2010
* All rights reserved.
* 
* filename:hi_character_encode.h
* 
* func:���ļ�ʵ����base64 url 3des 3des crc �ı����
*      md5���� �� ���������ֵ�ת��
* 
* 
*/

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int UINT4;

/* MD5 context. */
struct md5_ctx 
{
	UINT4 state[4];					/* state (ABCD) */
	UINT4 count[2];		                     /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];			 /* input buffer */
};

typedef struct md5_ctx MD5_CTX;

#define uchar   unsigned char
#define uint    unsigned int
#define ulong   unsigned long int

typedef struct
{
    ulong esk[32];     /* DES encryption subkeys */
    ulong dsk[32];     /* DES decryption subkeys */
}des_context;

typedef struct
{
    ulong esk[96];     /* Triple-DES encryption subkeys */
    ulong dsk[96];     /* Triple-DES decryption subkeys */
}des3_context;

/*****************************************************
md5 ���ܺ���

outbuffer:���ܺ���ַ���(�̶�����Ϊ32)
input: ����
*****************************************************/

/* md5 auth algo type */
typedef enum
{
    EN_MD5AUTH_ALGO_MD5,
    EN_MD5AUTH_ALGO_SESS,
    EN_MD5AUTH_ALGO_INVALID
}EN_MD5AUTH_ALGO;

void md5_encrypt(unsigned char *outbuffer, /* 16 bytes */
                const unsigned char *input);

#define HASHLEN         16
#define HASHHEALEXEN      32

typedef char HASH[HASHLEN];
typedef char HASHHEX[HASHHEALEXEN + 1];

void md5_auth_cvt_hex( HASH bin, HASHHEX hex );

void md5_auth_digest_calc_hA1(const char* authId, const char* password,
                             unsigned char alg, const char* realm, const char* nonce,
                             const char* cnonce, HASHHEX sessionKey );

void md5_auth_digest_calc_response( HASHHEX ha1, const char* nonce, const char* nc,
                                 const char* cnonce, const char* qop, const char* method,
                                 const char* uri, HASHHEX entity, HASHHEX response );

int md5_auth_build_resonse( char* resp, unsigned short maxlen,
                                     unsigned char alg, const char* auth, const char* realm,
                                     const char* pass, const char* nonce, const char* nc,
                                     const char* cnonce, const char* qop,
                                     const char* method,const char*  uri);

/*
 * DES-CBC decryption
 */
void des_cbc_decrypt( des_context *ctx, uchar iv[8],
                      uchar *input, uchar *output, uint len );

/*
 * DES-CBC encryption
 */
void des_cbc_encrypt( des_context *ctx, uchar iv[8],
                      uchar *input, uchar *output, uint len );

/*
 * Triple-DES 64-bit block encryption (ECB)
 */
void des3_encrypt(uchar *key, uchar input[8], uchar output[8] );

/*
 * Triple-DES 64-bit block decryption (ECB)
 */
void des3_decrypt(uchar *key, uchar input[8], uchar output[8]);

/*
 * 3DES-CBC encryption
 */
void des3_cbc_encrypt( des3_context *ctx, uchar iv[8],
                       uchar *input, uchar *output, uint len );

/*
 * 3DES-CBC decryption
 */
void des3_cbc_decrypt( des3_context *ctx, uchar iv[8],
                       uchar *input, uchar *output, uint len );

/****************************************************************
base64���뺯��

src:Դ�ַ���
src_len:Դ�ַ����ĳ���
dst:������ַ����Ĵ�ŵ�ַ����ȷ������ռ��С����3*src_len/4��
dst_len:���ؽ�����ַ����ĳ���
*****************************************************************/		
void base64_decode(unsigned char *src, int src_len, char *dst, int *dst_len);

/**************************************************************
base64���뺯��

src:Դ�ַ���
src_len:Դ�ַ����ĳ���
dst:������ַ����Ĵ�ŵ�ַ����ȷ������ռ��С����4*src_len/3��
dst_len:���ر�����ַ����ĳ���
***************************************************************/
void base64_encode(unsigned char *src, int src_len, char *dst, int *dst_len);

/****************************************************************
url���뺯��

s:Դ�ַ���(������ַ���Ҳ���ڸõ�ַ)
len:Դ�ַ����ĳ���
����ֵ���������ַ����ĳ���
***************************************************************/
int url_decode(char *str, int len);

/****************************************************************
url���뺯��

s:Դ�ַ���(������ַ���Ҳ���ڸõ�ַ)
len:Դ�ַ����ĳ���
����ֵ���������ַ����ĳ���
***************************************************************/
int url_decode(char *str, int len);

/***********************************************************
Hex ת Dec ����

uc:16������		
����ֵ���ɹ�����16��������Ӧ��ʮ��������ʧ�ܷ���-1
***********************************************************/
int	hex_2_dec(unsigned char uc);

/***********************************************************
16������תASCII�뺯��

pSrc:16������
len :16����������
pDest:ת�����ʮ������

����ֵ���ɹ�����ת�����ʮ�������ĳ��ȣ�ʧ�ܷ���-1
***********************************************************/
int	hex_2_asc(unsigned char *pSrc,int len,unsigned char *pDest);

/***********************************************************
Dec ת Hex ����

uc:10������		
����ֵ���ɹ�����10��������Ӧ��16��������ʧ�ܷ���-1
***********************************************************/
unsigned char dec_2_hex(int val);

/***********************************************************
ASCII��ת16����������

pSrc:ʮ������
len :ʮ����������
pDest:ת�����16������

����ֵ���ɹ�����ת�����16�������ĳ��ȣ�ʧ�ܷ���-1
***********************************************************/
int asc_2_hex(unsigned char *pSrc,int len,unsigned char *pDest);

/***********************************************************
BCD��תAsc�뺯��

pData:bcd ���ִ�

����ֵ���ɹ�����ת�����ֵ��ʧ�ܷ���-1
***********************************************************/
unsigned short bcd_2_asc(unsigned char *pData);

/***********************************************************
Asc��תBCD�뺯��

pData:Ascii ���ִ�

����ֵ���ɹ�����ת�����BCD��ֵ��ʧ�ܷ���-1
***********************************************************/
unsigned char asc_2_bcd(unsigned char pData);

/***********************************************************
��ȡCRCУ����

pData:У������
nLength:У�����ݳ���

����ֵ��CRCУ����
***********************************************************/
unsigned char check_crc(unsigned char * pData, int nLength);
#ifdef __cplusplus
}
#endif

#endif

