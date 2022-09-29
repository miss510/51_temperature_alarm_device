#ifndef __BUZZER_H__
#define __BUZZER_H__

#include <reg51.h>

// ���Ŷ���
sbit BUZZER = P2^4;  		// buzzer����������

// �궨��
// ���÷����������Ƶ��ΪXKHZ
//#define XKHZ	4 					// Ҫ������Khz����ֱ��д����
//#define US		(500/XKHZ)
//#define N 		(65535-US)

// N = (65535 - (500 / XKHZ))
//#define TIMELEN	600



// ��������

// ��ʼ����������Ĭ��������ǲ����
void buzzer_init(void);

// �÷�������ʼ��
void buzzer_start(void);
// �÷�����ֹͣ��
void buzzer_stop(void);
// ���÷��������Ƶ��
void buzzer_freq_set(unsigned char tmp);



#endif