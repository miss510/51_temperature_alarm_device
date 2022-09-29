// 开头2行和最后1行加起来构成一种格式，这种格式利用了C语言的预处理中的条件编译
// 技术，实现的效果就是防止该头文件被重复包含造成错误
#ifndef __UART_H__
#define __UART_H__



#include <reg51.h>



// 串口初始化函数
// 预设一个串口条件：8数据位、1停止位、0校验位、波特率9600
// 初始化的主要工作就是去设置相关的寄存器
// 使用晶振是11.0592MHz
// CPU工作在12T模式下
void uart_init(void);

// 通过串口发送1个字节出去
char putchar(unsigned char c);


#endif