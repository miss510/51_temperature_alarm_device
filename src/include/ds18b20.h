// ds18b20在普中开发板中的测试
// 本测试使用晶振为11.0592MHz
#ifndef __DS18B20_H__
#define __DS18B20_H__

#include<reg51.h>
					   

//--定义使用的IO口--//
sbit DATA = P3^7;



// 操作ds18b20并读取温度值，将温度值返回
double Ds18b20ReadTemp(void);


#endif









