#ifndef __1602_H__
#define __1602_H__

#include <reg51.h>
#include "ds1302.h"
#include <string.h>
#include <stdio.h>


// IO�ӿڶ���
#define LCD1602_DB  P0      //data bus ��������
// ��������
sbit LCD1602_RS = P2^6;
sbit LCD1602_RW = P2^5;
sbit LCD1602_EN = P2^7;	



// ֻ��Ҫ�����߲�ʱ�򼴿ɣ����Ͳ�ʱ���ǲ���Ҫ�����ġ�
// ��Ϊ������ͷ�ļ����������������Ŀ����Ϊ���ñ���ļ�ȥ�������ͷ�ļ�
// �Ӷ����Ե������ͷ�ļ��������ĺ�������������ֻ��Ҫ����1602.c�н���
// �ᱻ�ⲿ.c�ļ����õ���Щ�������ɡ���1602.c���Լ�ʹ�õ��ڲ���������Ҳ
// ���ᱻ�ⲿ.c�ļ����ã���˾Ͳ���������

// ��ʼ��LCD��ʹ֮�ܹ���ʼ��������
void InitLcd1602(void);

// ���ù��ָ��
void LcdSetCursor(unsigned char x,unsigned char y);

// �������������õ�ǰ���λ�ã���ʵ�������õ�ǰ���ڱ༭��λ�ã�
// ��ʵ�����ڲ������ݵ�ַָ�룬��ʵ����RAM�Դ��ƫ����
// x��Χ��0-15��y=0��ʾ����һ�У�y=1��ʾ����һ��
//void LcdSetCursor(unsigned char x,unsigned char y);

// ���������ǣ�������(x,y)��ʼ��ʾ�ַ���str
// ע������������ܿ�����ʾ����Ϊ�Դ��ַ�ǲ�������
// ��ʵ���ǿ��Է�װ��һ���ܹ�������ʾ�ĺ�����
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);

// ��ʾ������24.5���ֵ�double���͵�����
void LcdShowDouble(unsigned char x, unsigned char y, double d);

// LCD1602����ʾtime_t
void LcdShowTime(struct time_t mytime);

// ��LCD����ʾһ��1-16֮�������
void LcdShowInt(unsigned char x, unsigned char y, unsigned char d);

// ����LCD����ͨģʽ������ʾ��꣩
void LcdNormalMode(void);

// ����LCD������ģʽ����ʾ��꣬���ҹ����˸��
void LcdAjustMode(void);

#endif