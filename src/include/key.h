#ifndef __KEY_H__
#define __KEY_H__

#include <reg51.h>



// 4*4矩阵按键
#define 	KEY		P1


// 接口定义

// 调用该函数后会去进行按键扫描，如果没有人按按键则返回0，如果有人按按键
// 则返回按下按键的键值
unsigned char GetKey(void);






#endif