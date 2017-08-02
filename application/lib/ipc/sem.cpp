#include "sem.h"
#include "debug.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>

int GetSem(int SemID)
{
	struct sembuf sb;

	sb.sem_num = 0;
	sb.sem_op = -1;
	sb.sem_flg = SEM_UNDO;		/* �����쳣�˳���,�Զ��ͷ�,�������� */
	
	if (semop(SemID, &sb, 1) < 0)
	{
		LOG_ERROR("semop error\n");
		return -1;		
	}

	return 0;
}

int PostSem(int SemID)
{
	int ret;
	struct sembuf sb;

	sb.sem_num = 0;
	sb.sem_op = 1;
	sb.sem_flg = SEM_UNDO;		/* �����쳣�˳���,�Զ��ͷ�,�������� */

	//����һ����һ���ź�
	//��semopС��0 ʱ����ʾ��ȡ�ź���ʧ��
	if ((ret = semop(SemID, &sb, 1)) < 0)
	{
		LOG_ERROR("semop error[%d]\n", ret);
		perror("semop");
		return -1;
	}

	return 0;
}

int CreateSem(int SemKey)
{
	int SemID;
	union semun sem_union;  

	//����һ���µ��ź������ȡһ���Ѿ����ڵ��ź����ļ�ֵ��
	SemID = semget(SemKey, 1, IPC_CREAT | IPC_EXCL | 0666);
    if ((SemID < 0) && (errno == EEXIST))	/* �ź����Ѿ����� */
    {
		SemID = semget(SemKey, 1, IPC_CREAT | 0666);
		if (SemID < 0)
		{
	        LOG_ERROR("shmget error\n");
			return -1;	
		}
    }
	else if (SemID >= 0)	/* �ź���������,��������ʼ�� */
	{
		union semun sem_union;  

		sem_union.val = 1;  
		if (semctl(SemID, 0, SETVAL, sem_union) < 0) 
		{
			LOG_ERROR("error at semctl\n");
			return -1;  
		}
	}
	else
    {
        LOG_ERROR("shmget error\n");
		return -1;		
    }

	return SemID;  
}

int CleanSem(int SemID)
{
	union semun sem_union;  

	sem_union.val = 1;  
	if(semctl(SemID, 0, SETVAL, sem_union) < 0) 
	{
		LOG_ERROR("error at semctl\n");
		return -1;  
	}

	return 0;
}

int UninitSem(int SemID)
{
	union semun sem_union;  

	if(semctl(SemID, 0, IPC_RMID, sem_union) < 0)
	{
		LOG_INFO("error at del sem\n");
		return -1;
	}

	return 0;
}
