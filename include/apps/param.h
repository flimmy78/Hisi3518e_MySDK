#ifndef      __PARAM_H__
#define     __PARAM_H__

#ifdef	__cplusplus
extern "C" { 
#endif

#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/********************************   ����ֵ����   *****************************/
/* ��������������ͨ��hash��ת�����ַ��� */
#define		GET_INDEX(x)		(x * 0x100)		/* ��������֮���״ֵ̬�ı� */
typedef enum PARAM_VALUE_HASH_INDEX_T			/* (���ּ������)SDK��Ҫ */
{
    /* misc */
	OFF  = GET_INDEX(0),
	ON,
	
	IRLED_AUTO  = GET_INDEX(1),
	IRLED_ON,
	IRLED_OFF,	

	TEST_OK  = GET_INDEX(2),
	TEST_FAIL,    

    /* audio */
	AUDIO_TYPE_G711 = GET_INDEX(3),	    /* 10��ȡֵ */
	AUDIO_TYPE_G726,
	AUDIO_TYPE_ADPCM,
	AUDIO_TYPE_PCM,
	AUDIO_TYPE_AAC,

    /* video */
	FLIP_NORMAL = GET_INDEX(4),
	FLIP_H,
	FLIP_V,
	FLIP_HV,  

    QVGA = GET_INDEX(5),
	VGA,
	P720,
	P960,
	P1080, 

	F50HZ = GET_INDEX(6),
	F60HZ,    

	MAIN_STREAM_CH = GET_INDEX(7),
	SUB1_STREAM_CH,
	SUB2_STREAM_CH,
	SUB3_STREAM_CH,	    

    /* PTZ */
	SPEED_LOW = GET_INDEX(8),
	SPEED_NORMAL,
	SPEED_HIGH,

    /* user */
	USER_ADMIN = GET_INDEX(9),
	USER_OPT,
	USER_GUEST,
	USER_NULL,

    /* wifi */
	AUTH_WEP = GET_INDEX(10),	//2560
	AUTH_WPA,
	AUTH_WPA2,
	AUTH_OPEN,
	
	ENC_NONE = GET_INDEX(11),
	ENC_SHARE,
	ENC_AES,
	ENC_TKIP,
	
	WIFI_DISCONNECT = GET_INDEX(12),
	WIFI_CONNECT,	

	WIFI_LINKED = GET_INDEX(13),
	WIFI_UNLINKED,    

	/* timezone */
	GMT_12 = GET_INDEX(14),
	GMT_11,
	GMT_10,
	GMT_9,
	GMT_8,
	GMT_7,
	GMT_6,
	GMT_5,
	GMT_4,
	GMT_3,
	GMT_2,
	GMT_1,	
	GMT0,
	GMT1,		
	GMT2,	
	GMT3,	
	GMT4,		
	GMT5,	
	GMT6,	
	GMT7,		
	GMT8,	
	GMT9,	
	GMT10,	
	GMT11,		
	GMT12,	

    /* email */
	EMAIL_TLS = GET_INDEX(15),
	EMAIL_SSL,

	SMTP_SUCCESS = GET_INDEX(16),    

    /* time */
	NORMAL_SYNC = GET_INDEX(17),            /* �ֶ����� */
	SYNC_TO_PC,                             /* ��PCͬ��, �������ҳ���ͬ��һ�� */
	NTP_SYNC,
	
	NTP_SYNC_OK = GET_INDEX(18),
	NTP_SYNC_FAIL,	
	NTP_SYNC_OFF,

    /* network */
    USE_ETH = GET_INDEX(19),
    USE_WLAN,
    NET_TYPE_NULL,
    
    DHCP_IP = GET_INDEX(20),
    DHCP_ALL,
    DHCP_OFF,

    /* upnp */
    UPNP_FAIL = GET_INDEX(21),
    UPNP_OFF,    

    /* alarm */
	MD_EVENT = GET_INDEX(22),
    GPIO_EVENT,
    VIDEO_LOSS_EVENT,

	MD_LOW = GET_INDEX(23),       
	MD_NORMAL,
	MD_HIGHT,	
	MD_HIGHTEST,	    

    /* DDNS */
	DDNS_9299 = GET_INDEX(24),   /* 10��ȡֵ */
	DDNS_TWS,
    DDNS_EASYN,
	DDNS_NO_IP,
	DDNS_DYNDNS,
	DDNS_3322,

	UPDATE_OK = GET_INDEX(25),		        /* ���³ɹ� */	
	DDNS_UPDATE_FAIL,					    /* ����ʧ�� */
	TVS_DDNS_OFF,						    /* ����ddns�ر� */
	THR_DDNS_OFF,						    /* ������ddns�ر� */
	
	TVS_911_ERROR = GET_INDEX(26),		    /* ���� */
	TVS_911CC_ILLEGAL,					    /* �Ƿ��˺� */
	TVS_901_AUTH_FAIL,					    /* �û���֤���� */
	TVS_DELETE_OK,						    /* ɾ�����³ɹ� */
	
	EASYN_SERVER_NOAUTH = GET_INDEX(27),    /* û��Ȩ�� */
	EASYN_SERVER_NOID,                      /* �˺Ų����� */
	EASYN_SERVER_OVER,	                    /* �˺��ѹ��� */
	EASYN_SERVER_ERR_IDDISABLE,	            /* �˺��ѱ����� */
	EASYN_SERVER_ERR_PARAM,	                /* �������� */

    /* ftp */
	PASV_MODE = GET_INDEX(28),              /* ����ģʽ */
	PORT_MODE,                              /* ����ģʽ */

	FTP_SUCCESS = GET_INDEX(29),    	

    /* p2p */
    TUTK_P2P = GET_INDEX(30),
    ILNK_P2P,
    DANALE_P2P,

    /* led */
    LED_OFF = GET_INDEX(31), /*blue light*/
    LED_ON,					/*red light*/
    LED_FLASH_SLOW,			/*red or blue slow*/
    LED_FLASH_QUICK,		/* red or blue quick*/
    LED_BLUE_FLASH,			/*blue flash*/
    LED_RED_FLASH,			/*red flash*/
	
	/*wifi status*/
	NET_IDLE = GET_INDEX(32),	//8192
	NET_AP,
	NET_SMART_LINK,
	NET_LINKING,	//connect wifi
	NET_LINKED,
	NET_UNLINKED,

	LED_CTL_AUTO = GET_INDEX(33),
	LED_CTL_MANUL,
	
}PARAM_VALUE_HASH_INDEX;

/************************    ���β���ȡֵ��Χ     *************************/
typedef enum NET_PORT_T
{
	MIN_PORT = 1,
	MAX_PORT = 65535,
}NET_PORT;

typedef enum VIDEO_FPS_LIMIT_T
{
	MIN_FPS = 1,
	MAX_FPS = MAX_FRAME_RATE,
}VIDEO_FPS_LIMIT;

typedef enum VIDEO_BITRATE_LIMIT_T
{
	MIN_BITRATE = 100,				/* 100K bps */
	MAX_BITRATE = 8192,				/* 8M bps */
}VIDEO_BITFRATE_LIMIT;

typedef enum VIDEO_I_PEROID_LIMIT_T
{
	MIN_I_PERIOD = 1,
	MAX_I_PERIOD = 255,
}VIDEO_I_PEROID_LIMIT;

typedef enum AUDIO_VOL_LIMIT_T
{
	MIN_VOL = 0,
	MAX_VOL = 100,
}AUDIO_VOL_LIMIT;

typedef enum MOTOR_POS_LIMIT_T
{
	PTZ_MIN_X = 0,
	PTZ_MIN_Y = 0,		
	PTZ_MAX_X = 2048,
	PTZ_MAX_Y = 512,
}MOTOR_POS_LIMIT;

typedef enum MOTION_DETECT_SENS_E
{
	MD_SENSITIVITY_0 = 50,
	MD_SENSITIVITY_1 = 150,
	MD_SENSITIVITY_2 = 250,
	MD_SENSITIVITY_3 = 400,
	MD_SENSITIVITY_4 = 600,
	MD_SENSITIVITY_5 = 850,
	
}MOTION_DETECT_SENS;

typedef enum SAVING_TIME_LIMIT_T
{
	MIN_TIME = 10,
	MAX_TIME = (3 * 60),
}SAVING_TIME_LIMIT;

typedef enum PTZ_POS_LIMIT_T
{
    MIN_POS = 1,
    MAX_POS = 8,
}PTZ_POS_LIMIT;

typedef struct TVS_DDNS_INFO_T
{
    char ddns_server[STR_64_LEN];
    int server_port;    
    char user[STR_32_LEN];    
    char pwd[STR_32_LEN]; 
    
    char acod[STR_32_LEN];
}TVS_DDNS_INFO;

typedef struct  EASYN_DDNS_INFO_T
{
    char ddns_server[STR_64_LEN];
    int server_port;    
    char user[STR_32_LEN];    
    char pwd[STR_32_LEN];

    char vertype[STR_32_LEN];
    int language;
    int dtype;        /* 0 */
    int tcpport;      /* web port */
    char lanip[STR_32_LEN];
}EASYN_DDNS_INFO;

typedef struct INNER_DDNS_PARAM_T
{
    int type;
    int enable;
    union
    {
        TVS_DDNS_INFO TvsDdnsInfo;
        EASYN_DDNS_INFO EasynDdnsInfo;    
    };
}INNER_DDNS_PARAM;

/*******************************   �����ṹ�嶨��   ****************************/
/* flash ϵͳ���ò����ṹ�� */
typedef struct FTP_PARAM_T
{
    char server[STR_64_LEN];
    int port;
    char user[STR_64_LEN];
    char pwd[STR_64_LEN];
    char upload_path[STR_64_LEN * 2];
	int mode;	
	int autocover;
}FTP_PARAM;

typedef struct EMAIL_PARAM_T
{
    char server[STR_32_LEN];
    char sender[STR_32_LEN];	
    char user[STR_32_LEN];		/* �ݲ�ʹ��,Ĭ����sender��ͬ */
    char pwd[STR_32_LEN];
    char recv1[STR_32_LEN];	
    char recv2[STR_32_LEN];		
    char recv3[STR_32_LEN];	
    char recv4[STR_32_LEN];		
    int port;
	int mode;
	char subject[STR_64_LEN];
	char context[STR_256_LEN];
}EMAIL_PARAM, SMTP_CONF;

typedef struct STREAM_PARAM_T
{
	int resolution;
    int fps;
    int idr;
    int bitrate;
}STREAM_PARAM;

typedef struct VIDEO_PARAM_T
{   
    STREAM_PARAM stream[MAX_STREAM_CNT];
	int flip;				/* ͼ��ת */
	int wideDynamic;		/* ��̬ */
	int osd_name_enb;
	int osd_time_enb;
	int brightness;		    /* ���� */
	int contrast;			/* �Աȶ� */
	int hue;				/* ɫ�� */
	int saturation;		    /* ���Ͷ� */
	int quality;
}VIDEO_PARAM;

typedef struct RECORD_PEROID_T
{
	char start_time[16];
	char end_time[16];
	int day[7];    
	int stepoverflag;	//����ʱ���Ƿ����
}RECORD_PEROID;

typedef struct RECORD_PARAM_T
{
	int enable;
	int stream_ch;
	int peroid;
	char sch[7][STR_64_LEN];	/* 30min һ��Ƭ��,ʵ��ʹ��48���ֽ� */
	/* �����ճ� for danale */
	char start_time[32];
	char end_time[32];
	int day[7];  
	//normal record
	RECORD_PEROID normal_rec_plan;
	
}RECORD_PARAM;

typedef struct SNAP_PARAM_T
{
	int enable;
	int sd_enable;	 	    /* sd���洢 */
	int sd_peroid;		    /* �洢���(�����ļ����Ч) */
	int email_enable;		/* email���� */
	int email_peroid;		/* ���ͼ�� */
	int ftp_enable;		    /* ftp�ϴ� */
	int ftp_peroid;		    /* �ϴ���� */
}SNAP_PARAM;

typedef struct AUDIO_PARAM_T
{
    int type;
    int in_vol;
    int out_vol;
    int mic_enable;
    int samplerate;
    int bitwidth;
    int bitrate;
    int channel;
}AUDIO_PARAM;

typedef struct DDNS_PARAM_T
{
	int enable;
    int type;
	char domain[STR_64_LEN];
	char server[STR_64_LEN];
	char user[STR_64_LEN];
	char pwd[STR_64_LEN];
}DDNS_PARAM;

typedef struct ROI_T
{
	unsigned short x;	  		        /* horizontal offset */
	unsigned short y;	  		        /* vertical offset */
	unsigned short width;
	unsigned short height;
}ROI;

typedef struct MD_INFO_T
{
	ROI roi;
	int enable;
	int sensitivity;
	int threshold;
}MD_INFO;

typedef struct MOTOR_POS_T
{
	int x;
	int y;
}MOTOR_POS;

typedef struct PTZ_PARAM_T
{
	int speed;
	int start_pos;
	int circle_cnt;   
	MOTOR_POS motor_pos[MAX_POS_CNT];
}PTZ_PARAM;

typedef struct TIME_PARAM_T
{
	int sync_type;				/* ʱ��ͬ����ʽ */
	char ntp_server[STR_64_LEN];	/* ntp��������ַ */
	int sync_interval;			/* ntpͬ�����(СʱΪ��λ) */
	int saving_time;				/* ����ʱʱ��(����Ϊ��λ) */
	int enb_savingtime;		    /* ����ʱʹ�� */
	int timezone;					/* ʱ�� */
    char timezone_city[STR_64_LEN];           /* ��Ӧ���е����� */
}TIME_PARAM;

typedef struct NET_PARAM_T
{
	int dhcp;
	char ip[STR_64_LEN];
	char netmask[STR_64_LEN];
	char gateway[STR_64_LEN];
	char dns1[STR_64_LEN];
	char dns2[STR_64_LEN];	
	char mac[STR_32_LEN];   
    int used_inf;                 /* ? */
}NET_PARAM;

typedef struct USER_PARAM_T
{
	int level;
	char user[STR_64_LEN];
	char pwd[STR_64_LEN];	
}USER_PARAM;

typedef struct MULTI_PARAM_T
{
    char alias[STR_64_LEN];
	char ip[STR_64_LEN];  
	char user[STR_64_LEN];
	char pwd[STR_64_LEN];
	int port;      
}MULTI_PARAM;

typedef struct WIFI_PARAM_T
{
    int enable;    
	int auth;		                /* WEP/WPA/WPA2 */
	int enc;		                /* SHARE/NONE/AES/TKIP */
	char ssid[STR_64_LEN];
	char key[STR_64_LEN];
}WIFI_PARAM;

typedef struct ALARM_PARAM_T		
{
	ROI roi[MD_ROI_CNT];                        /* ��δʹ�� */
    char roi_str[MD_ROI_CNT][STR_32_LEN];       /* ��δʹ�� */
	int enable[MD_ROI_CNT];                   /* ֻʹ��enable[0] */
	int sensitivity[MD_ROI_CNT];              /* ֻʹ��sensitivity[0] */
	int alarm_pos;		                    /* ������λ */
#if 0                                           /* һ��Ҫ��int����,������set_paramֵ�Ḳ�� */
    unsigned char alarm_bit[30];                 /* ������������ */
#else
    int alarm_bit[30];
#endif
    
	int ptz_enb;	                /* ��̨��������(reserved) */
	int sound_enb;                /* �������� */
	int ftp_pic;                  /* ftp�ϴ� */
	int email_pic;                /* email���� */
	int pic_to_sd;
	int rec_to_sd;
	int rec_to_ftp;    
	int pic_cnt;
    
	int sch_enb;		            /* �����ճ� */
	char sch_day[7][STR_64_LEN];	/* 30min һ��Ƭ��,ʵ��ʹ��48���ֽ� */	
	//alarm record
	RECORD_PEROID alarm_rec_plan;
	int period;
	
}ALARM_PARAM;

typedef struct RTMP_PARAM_T
{
    int push_enb;
    int port;
    char push_url[STR_256_LEN];
}RTMP_PARAM;

typedef struct MISC_PARAM_T
{
    /* read only param */
	char soft_version[STR_32_LEN];
    char uboot_version[STR_32_LEN];
    char kernel_version[STR_32_LEN];    
	char git_head[STR_64_LEN];				/* git(svn) �汾 */    
    /* read and write param */
	char  name[STR_32_LEN];
	int stream_index;						/* web������ķֱ��� */
	int pwr_freq;
	int ir_led;
	int http_port;
	int rtsp_port;
	int onvif_port;
	int p2p_enable;
	int upnp_enable;
    int app_video_quality;
	char ocx_rec_path[STR_128_LEN];
    char web_debug_path[STR_128_LEN];
    char osd[STR_64_LEN];
}MISC_PARAM;

typedef struct VIDEO_PARAM_INFO_T
{
    int resolution;
    int bitrate;
    int fps;
}VIDEO_PARAM_INFO;

typedef struct LED_STATUS_ONOFF_T
{
	unsigned int wifi_led_status;
	unsigned int ir_led_status;
}LED_STATUS_ONOFF;

typedef struct FLASH_PARAM_T
{
	int magic_num;
	
    FTP_PARAM ftp_param;
    EMAIL_PARAM email_param;
	ALARM_PARAM alarm_param;
    VIDEO_PARAM video_param;
    AUDIO_PARAM audio_param;
	NET_PARAM net_param;
	WIFI_PARAM wifi_param;
	DDNS_PARAM ddns_param;
	TIME_PARAM time_param;
	USER_PARAM user_param[MAX_USER_CNT];
	MULTI_PARAM multi_param[MAX_CAM_CNT];
	SNAP_PARAM snap_param;
	RECORD_PARAM record_param;
	PTZ_PARAM ptz_param;
	MISC_PARAM misc_param;
    RTMP_PARAM rtmp_param;
	LED_STATUS_ONOFF led_status_onoff;
//    VIDEO_PARAM_INFO p2p_stream_param;
}FLASH_PARAM;
/* end flash ϵͳ���ò����ṹ�� */

/* ram ϵͳ���ò����ṹ�� */
typedef struct SYSTEM_STATUS_T
{
	int inner_ddns_status;
	int thr_ddns_status;	
	int wifi_status;
	int upnp_status;
	int ntp_status;
	int nfs_mount_status;
	int talk_alive;
	int net_inf;	
	int wifi_test_result;
	int ftp_test_result;
	int email_test_result;
	char wlan_ip[STR_64_LEN];
	char upnp_wlan_ip[STR_64_LEN];
	char start_time[STR_64_LEN];
	int run_time;
	int upgrade;        /* ������ʱ����1, ����task_check����ϵͳ */
}SYSTEM_STATUS;

typedef struct SENSOR_INFO_T
{
	int temp;
	int humidity;			/* ʪ�� */
}SENSOR_INFO;

typedef struct SPS_INFO_T
{
    int len;
    unsigned char data[STR_32_LEN];
}SPS_INFO;

typedef struct PPS_INFO_T
{
    int len;
    unsigned char data[STR_32_LEN];
}PPS_INFO;

typedef struct AVC_HEADER_T
{
    SPS_INFO sps;
    PPS_INFO pps;
}AVC_HEADER;

typedef struct OTA_PARAM_T
{
    char version_description[1024];
    char lastest_version[32];
    int need_upgrade;
	unsigned char update_process;
	char update_statue;
	char connect_server;
	char update_filename[64];//[10];
	char isnew_version;
}OTA_PARAM;

typedef struct RECORDING_PATH_T
{
	char normal[STR_64_LEN];
	char alarm[STR_64_LEN];
	int mRecIndexOptFlag;	//��ֹ����̲߳���¼�������ļ�
	
}RECOREING_PATH;

typedef struct RAM_PARAM_T
{
	int init; 
	int smart_link_flag;
	int mpp_init_flag;
	int key;
	int led;    
	int wifi_mode;      /* 0: station mode / 1: ap mode */
	int again;          /* ispģ������ */
	int ispdgain;
	NET_PARAM net_param;
	SENSOR_INFO sensor_info;
	unsigned char aac_sequence_header[2];
	AVC_HEADER avc_header[MAX_STREAM_CNT];
	int UpgradeMsgHandle;
	int real_fps[MAX_STREAM_CNT];
	OTA_PARAM ota_param;
	int net_status;
	int airlink_flag;
	RECOREING_PATH recpath;

	SYSTEM_STATUS system_status;
}RAM_PARAM;
/* end ram ϵͳ���ò����ṹ�� */

/* factory ϵͳ���ò����ṹ�� */
typedef struct FACTORY_PARAM_T
{
	int magic_num;

	/* read only param */
	char author[STR_64_LEN];       
	/* read and write param */
	char hard_version[STR_32_LEN];	    
	char uid[STR_64_LEN];
	char sn[STR_32_LEN];
	char inner_ddns_enb;
	int limit_x;
	int limit_y; 
	int p2p_type;    
	char model[STR_32_LEN]; 
	char oem[STR_64_LEN]; 
	char sensor[STR_32_LEN];
	/* for online upgrade */
	char region[STR_32_LEN];
	char version_desc[STR_256_LEN];
	INNER_DDNS_PARAM ddns_param;    
}FACTORY_PARAM;
/* end factory ϵͳ���ò����ṹ�� */

/* start ϵͳ�������� */
typedef struct SYSTEM_PARAM_T
{
    struct FACTORY_PARAM_T factory_param;
    struct FLASH_PARAM_T flash_param;
    struct RAM_PARAM_T ram_param;
}SYSTEM_PARAM;
/* end ϵͳ�������� */

/* =================================   SDK�ⲿʹ��   ============================= */
/********************************   �������Ͷ���   **************************/
/* ����ӵĺ����β�����,�������ϼ��� */
typedef enum PARAM_TYPE_LIST_T					/* (���ּ������)SDK��Ҫ */
{
    TYPE_FTP_PARAM = 0,
    TYPE_EMAIL_PARAM,        
    TYPE_ALARM_PARAM,
    TYPE_VIDEO_PARAM,
    TYPE_AUDIO_PARAM,
    TYPE_STATIC_NET_PARAM,
    TYPE_WIFI_PARAM,
    TYPE_THR_DDNS_PARAM,
    TYPE_TIME_PARAM,
    TYPE_USER_PARAM,
    TYPE_MULTI_PARAM,
    TYPE_SNAP_PARAM,
    TYPE_RECORD_PARAM,
    TYPE_PTZ_PARAM,
    TYPE_MISC_PARAM,
    MAX_PAARM_INDEX,
}PARAM_TYPE_LIST;

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif  /* __COMMON_H__ */
