#include "task/task_video.h"
#include "task/task_rtsp.h"
#include "task/task_ptz.h"

#include "av_buffer.h"
#include "my_timer.h"
#include "param_ext.h"
#include "param_base.h"

#include "boot_save_default.h"
#include "sem.h"
#include "string_parser.h"
#include "wifi.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/reboot.h>
#include <signal.h>


void term_handle(int signum)
{
	LOG_INFO("enter term_handle\n");

	wifi_uninit();					/* û����wifi_init,�˴�����ֶδ��� */
	system("killall udhcpc");		/* ����ر�dhcpc,����http,rtsp�ȶ˿��޷��ͷ� */
	system("killall udhcpd");
	//system("echo \"stop\" > /proc/smart_connection");
		
	exit(0);
}


int main(int argc, char *argv[])
{
	int ch;
	int isDaemon = 1;
	int system_init = 1;	

	signal(SIGTERM, term_handle);	
	signal(SIGINT , term_handle);		/* CTRL+C */

	/* ����ϵͳֻ����һ�� */
	if (system_init)	/* ϵͳĬ����main_enter��ʼ�� */
	{
		CreateSem(KEY_SEM_BUFFER);
		CreateSem(KEY_SEM_VGA_BUFFER);
		CreateSem(KEY_SEM_GPIO_CTRL);
	}

	if (string_parser_init() < 0)
		LOG_WARN("string_parser_init fail\n");
	if (wifi_init() < 0)
		LOG_WARN("wifi_init fail\n");
	if (init_timer() < 0)
		LOG_WARN("init_timer fail\n");
	if (av_buffer_init() < 0)
		LOG_WARN("av_buffer_init fail\n");

	/* ����ϵͳֻ����һ�� */
	if (system_init)
	{
		LOG_INFO("ready into sys_boot_config\n");
		sys_boot_config();	
	}

	//AjustTZ();

	while (param_is_init() == 0)	/* �ȴ������������ */
		usleep(100 * 1000);

	LOG_INFO("!!The param is init!!\n");

	ptz_task_create();	
	video_task_create();
	rtsp_task_create(); 

	LOG_INFO("all platform task has create, enter sleep loop\n");

	while(1)
	{
		sleep(1);
	}

	ptz_task_destory();
	video_task_destory();
	rtsp_task_destory();
	
	av_buffer_uninit();
	uninit_timer();
	wifi_uninit();
	string_parser_uninit();	
	return SDK_OK;
}

