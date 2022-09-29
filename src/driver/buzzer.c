#include "buzzer.h"


static unsigned int count;
static unsigned char flag = 0;			// flag = 0��ʾ��������flag = 1��ʾû����
//static int xKHz  = 1;					// Ĭ����1KHz
static unsigned char th0;
static unsigned char tl0;
/*
// timer0��isr������������Ž��е�ƽ��ת���÷�������
void timer0_isr(void) interrupt 1 using 1
{
	TL0 = N % 256;
	TH0 = N / 256;

	if (count-- == 0)
	{
	  	// ˵�����˷�ת��ʱ����
	//	count = 600;

		if (flag == 0)
		{
		   	// ֮ǰ�Ǵ����������ģ�˵�������Ǵ����������������ķ�ת
			flag = 1;
			count = TIMELEN*10;	 	// �����count�������������������ʱ�䳤��
		}
		else
		{
			// ֮ǰ�Ǵ���û�����ģ�˵�������Ǵ�û�������������ķ�ת
			flag = 0;
			BUZZER = !BUZZER;
			count = TIMELEN;   		// �����count�����������������ʱ�䳤��
		}
	}
	else
	{
		// ���������Ҳ���ǲ���תʱ
		if (flag == 0)
		{
			BUZZER = !BUZZER;			// 4999������
		}
		else
		{
		   	// �յľͿ��ԣ���Ϊ�������κ�IO��������û����
		}
	}
	
}
*/

// ���Ʒ��������isr
void timer0_isr(void) interrupt 1 using 1
{
	TH0 = th0;
	TL0 = tl0;

	// ���������Ҳ���ǲ���תʱ
	if (flag == 0)
	{
		BUZZER = !BUZZER;			// ��
	}
}

// buzzer�ĳ�ʼ������
void buzzer_init(void)
{
	TMOD = 0x01;		// T0ʹ��16bit��ʱ��
	tl0 = (65535 - (500 / 1)) % 256;
	th0 = (65535 - (500 / 1)) / 256;

	TH0 = th0;
	TL0 = tl0;

	TR0 = 1; 			// T0�򿪿�ʼ����
	ET0 = 1;	 		// T0�ж�����
	EA = 1;				// ���ж�����

	BUZZER = 1;

	// flag=1��Ĭ�Ϸ���������
	flag = 1;

//	xKHz = 1;			// Ĭ����4Khz
}

void buzzer_start(void)
{
	flag = 0;
}

void buzzer_stop(void)
{
	flag = 1;
}

void buzzer_freq_set(unsigned char tmp)
{
	tl0 = (65535 - (500 / tmp)) % 256;
	th0 = (65535 - (500 / tmp)) / 256;
}


