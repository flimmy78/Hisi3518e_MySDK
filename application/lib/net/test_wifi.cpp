#include "wifi.h"
#include "debug.h"
#include "param.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include <termios.h>
#include <unistd.h>



static int StdinDevInit(void)
{
    struct termios tTTYState;
 
    //get the terminal state
    tcgetattr(STDIN_FILENO, &tTTYState);
 
    //turn off canonical mode
    tTTYState.c_lflag &= ~ICANON;
    //minimum of number input read.
    tTTYState.c_cc[VMIN] = 1;   /* ��һ������ʱ�����̷��� */

    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &tTTYState);

	return 0;
}


static int StdinDevExit(void)
{

    struct termios tTTYState;
 
    //get the terminal state
    tcgetattr(STDIN_FILENO, &tTTYState);
 
    //turn on canonical mode
    tTTYState.c_lflag |= ICANON;
	
    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &tTTYState);	
	return 0;
}




int main(int argc, char *argv[])
{	
	int AP_Num = 0;
	char *str = NULL;//����洢��SSID
	int net_id = 0;
	int net_return = 0;

	char *wifi_value[5];//����������д����bssid, freq, signal, flags, ssid


	if (wifi_init() < 0)
	{
		LOG_ERROR("wifi_init fail\n");
		goto  OUT;
	}
	
	StdinDevInit();
	while(1)
	{
		//������Ҫ��ɨ�������ź�
		//���Ѿ����ӵ��źŽ����˺ź�����ı���
		if(wifi_check_link_Y_N() < 0)
		{
			LOG_ERROR("check_link_Y_N fail\n");
			continue;
		}
		if (wifi_scan() < 0)
		{
			LOG_ERROR("wifi_scan fail\n");
			continue;
		}
		/* ��ӡ���� */
		if (wifi_scan_result() < 0)
		{
			LOG_ERROR("wifi_scan_result fail\n");
			continue;
		}
        /* ���û�ѡ��ĳ��AP */
		if(wifi_Enter_Ssid(wifi_value,&AP_Num)<0)
		{
			LOG_ERROR("wifi_Enter_Ssid\n");
			continue;
		}
		/* ����һ��network */
		net_id = wifi_add_network();
		LOG_INFO("net_id [%d]\n", net_id);
		wifi_set_network(net_id, "ssid" , wifi_value[4]);

		/* ���û��������� */	
		LOG_INFO("User is going to input the password\n");
		if(wifi_Enter_Password(wifi_value,net_id)<0)
		{
			LOG_ERROR("wifi_Enter_Password\n");
			continue;
		}
	}
	
OUT:
	wifi_uninit();
	StdinDevExit();

	return 0;
}


