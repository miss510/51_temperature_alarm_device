#include "buzzer.h"


static unsigned int count;
static unsigned char flag = 0;			// flag = 0表示有声音，flag = 1表示没声音
//static int xKHz  = 1;					// 默认是1KHz
static unsigned char th0;
static unsigned char tl0;
/*
// timer0的isr，在这里对引脚进行电平反转以让蜂鸣器响
void timer0_isr(void) interrupt 1 using 1
{
	TL0 = N % 256;
	TH0 = N / 256;

	if (count-- == 0)
	{
	  	// 说明到了翻转的时候了
	//	count = 600;

		if (flag == 0)
		{
		   	// 之前是处于有声音的，说明本次是从有声音到无声音的翻转
			flag = 1;
			count = TIMELEN*10;	 	// 这里的count数量决定蜂鸣器不响的时间长短
		}
		else
		{
			// 之前是处于没声音的，说明本次是从没声音到有声音的翻转
			flag = 0;
			BUZZER = !BUZZER;
			count = TIMELEN;   		// 这里的count数量决定蜂鸣器响的时间长短
		}
	}
	else
	{
		// 常规情况，也就是不反转时
		if (flag == 0)
		{
			BUZZER = !BUZZER;			// 4999次声音
		}
		else
		{
		   	// 空的就可以，因为不进行任何IO操作就是没声音
		}
	}
	
}
*/

// 控制蜂鸣器响的isr
void timer0_isr(void) interrupt 1 using 1
{
	TH0 = th0;
	TL0 = tl0;

	// 常规情况，也就是不反转时
	if (flag == 0)
	{
		BUZZER = !BUZZER;			// 响
	}
}

// buzzer的初始化函数
void buzzer_init(void)
{
	TMOD = 0x01;		// T0使用16bit定时器
	tl0 = (65535 - (500 / 1)) % 256;
	th0 = (65535 - (500 / 1)) / 256;

	TH0 = th0;
	TL0 = tl0;

	TR0 = 1; 			// T0打开开始计数
	ET0 = 1;	 		// T0中断允许
	EA = 1;				// 总中断允许

	BUZZER = 1;

	// flag=1，默认蜂鸣器不响
	flag = 1;

//	xKHz = 1;			// 默认是4Khz
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


