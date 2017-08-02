#ifndef		__MOTOR_H__
#define		__MOTOR_H__

#include "param.h"
#include "common.h"

#ifdef	__cplusplus
extern "C" { 
#endif

typedef enum MOTOR_STATE_T
{
	STATE_START,		/* ��ʼ״̬ */
	STATE_EDGE,			/* ȥ�߽� */
	STATE_ORG,			/* ��ԭ�� */
	STATE_CENTER,		/* ȥ�е� */
	STATE_UP,
	STATE_DOWN,
	STATE_LEFT,
	STATE_RIGHT,
	STATE_RUN_ALL,		/* Ѳ�� */
	STATE_RUN_POS,		/* ��λ */
	STATE_STOP,
	STATE_BUTT,
}MOTOR_STATE;

/* [up/down/left/right] */
int motor_do_cmd(const char *cmd);
/* [go_to_pos/get_pos] */
int motor_pos_ctrl(int method, MOTOR_POS *pMotorPos);
int motor_set_speed(int speed);
int motor_set_circle(int cnt);
int motor_set_limit(int x, int y);
int get_motor_status(void);   /* 0: stop 1: run */

#ifdef	__cplusplus
}
#endif

#endif
