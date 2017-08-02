#ifndef		__TASK_GPIO_H__
#define		__TASK_GPIO_H__

#include "task_base.h"
#include "common.h"
#include "my_timer.h"

typedef enum ADC_LIMIT_T
{
    ADC_UP_LIMIT = 0xe,    /* IRLED �ر�(�л�������ģʽ) */
    ADC_DOWN_LIMIT = 0x7,  /* IRLED ��(�л���ҹ��ģʽ) */
    AGAIN_LIMIT = 13 * 1024,    /* again ���� AGAIN_LIMIT, ���л���ҹ��ģʽ*/
}ADC_LIMIT;

typedef enum VIDEO_MODE_T
{
    GRAY_MODE = 0,      /* �Ҷ�ģʽ */
    COLOR_MODE = 1,     /* ��ɫģʽ */
}VIDEO_MODE;

class CTaskGpio: public CTaskBase
{
    public:
		static CTaskGpio *getInstance()
		{
			static CTaskGpio *obj = NULL;
			if (NULL == obj)
			{
				obj = new CTaskGpio();		
			}
			return obj;
		}

        CTaskGpio();
        ~CTaskGpio();
		int Init(void);
		void Uninit(void);       
		void Process(void);  

    public:
        int mMode;
        int mGpioValue;
        TIMER *mLedTimer;
};

int gpio_task_create(void);
void gpio_task_destory(void);

#endif
