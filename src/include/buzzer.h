#ifndef __BUZZER_H__
#define __BUZZER_H__

#include <reg51.h>

// 引脚定义
sbit BUZZER = P2^4;  		// buzzer的驱动引脚

// 宏定义
// 设置蜂鸣器的输出频率为XKHZ
//#define XKHZ	4 					// 要定多少Khz，就直接写这里
//#define US		(500/XKHZ)
//#define N 		(65535-US)

// N = (65535 - (500 / XKHZ))
//#define TIMELEN	600



// 函数声明

// 初始化蜂鸣器，默认情况下是不响的
void buzzer_init(void);

// 让蜂鸣器开始响
void buzzer_start(void);
// 让蜂鸣器停止响
void buzzer_stop(void);
// 设置蜂鸣器响的频率
void buzzer_freq_set(unsigned char tmp);



#endif