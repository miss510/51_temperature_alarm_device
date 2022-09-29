#ifndef __1602_H__
#define __1602_H__

#include <reg51.h>
#include "ds1302.h"
#include <string.h>
#include <stdio.h>


// IO接口定义
#define LCD1602_DB  P0      //data bus 数据总线
// 控制总线
sbit LCD1602_RS = P2^6;
sbit LCD1602_RW = P2^5;
sbit LCD1602_EN = P2^7;	



// 只需要声明高层时序即可，而低层时序是不需要声明的。
// 因为我们在头文件中声明这个函数，目的是为了让别的文件去包含这个头文件
// 从而可以调用这个头文件中声明的函数，所以我们只需要声明1602.c中将来
// 会被外部.c文件调用的那些函数即可。而1602.c中自己使用的内部函数将来也
// 不会被外部.c文件调用，因此就不用声明了

// 初始化LCD，使之能够开始正常工作
void InitLcd1602(void);

// 设置光标指针
void LcdSetCursor(unsigned char x,unsigned char y);

// 本函数用来设置当前光标位置，其实就是设置当前正在编辑的位置，
// 其实就是内部的数据地址指针，其实就是RAM显存的偏移量
// x范围是0-15，y=0表示上面一行，y=1表示下面一行
//void LcdSetCursor(unsigned char x,unsigned char y);

// 函数功能是：从坐标(x,y)开始显示字符串str
// 注意这个函数不能跨行显示，因为显存地址是不连续的
// 其实我们可以封装出一个能够折行显示的函数的
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);

// 显示类似于24.5这种的double类型的数字
void LcdShowDouble(unsigned char x, unsigned char y, double d);

// LCD1602上显示time_t
void LcdShowTime(struct time_t mytime);

// 在LCD上显示一个1-16之间的数字
void LcdShowInt(unsigned char x, unsigned char y, unsigned char d);

// 设置LCD到普通模式（不显示光标）
void LcdNormalMode(void);

// 设置LCD到调整模式（显示光标，并且光标闪烁）
void LcdAjustMode(void);

#endif