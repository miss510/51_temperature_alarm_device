// ds18b20�����п������еĲ���
// ������ʹ�þ���Ϊ11.0592MHz
#ifndef __DS18B20_H__
#define __DS18B20_H__

#include<reg51.h>
					   

//--����ʹ�õ�IO��--//
sbit DATA = P3^7;



// ����ds18b20����ȡ�¶�ֵ�����¶�ֵ����
double Ds18b20ReadTemp(void);


#endif









