#include "uart.h"
#include "ds18b20.h"
#include "1602.h"
#include "ds1302.h"
#include "buzzer.h"
#include "key.h"




void delay1s(void)   //��� 0us
{
    unsigned char a,b,c;
    for(c=167;c>0;c--)
        for(b=171;b>0;b--)
            for(a=16;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
}

const unsigned char du[3] = {0xdf, 'C', '\0'}; 	// LCD1602��ʾ���϶ȷ����õ�


void main(void)
{
	uart_init(void);
	printf("sansan\r\n");
//	unsigned char key = 0;
//	// ���Ծ��󰴼�
//	InitLcd1602();
//	LcdShowStr(0, 0, "sansan");
//	key = GetKey();
//	if(key > 0)
//	{
//		LcdShowInt(0, 0, key);
//	}
	
	while(1);

	
/*
   	while (1)
	{
		KeyWorModeAjust();
		KeyCursorMove();
		//delay300ms();
	}
*/

/*
  // ����IR
  	IrInit();
  	InitLcd1602();
	LcdShowStr(0, 0, "tempw:"); 
  	while (1)
  	{
  		LcdShowInt2(8, 0, IrValue[4]);
  	}
*/

 /*
	unsigned char key = 0;

 	// ���Ծ��󰴼�
	InitLcd1602();
	LcdShowStr(0, 0, "temp:"); 
	LcdShowInt2(6, 0, 0);

	while (1)
	{
		key = GetKey();

		if (key != 0)
		{
			LcdShowInt2(6, 0, key);
		}
	}
*/


/*
	// �����������
	motor_alarm_once();
	while (1);
 */

/*
	// ����1602��ʾtime_t
	struct time_t t1;

	t1.year = 2017;
	t1.mon = 4;
	t1.date = 6;
	t1.hour = 15;
	t1.min = 1;
	t1.sec = 23;
	t1.day = 4;

	LcdShowTimeT(0, 0, t1);

	while (1);
*/

/*
   	// ����������
   	buzzer_init();
	buzzer_start();
	
	delay1s();
	buzzer_stop();

	delay1s();
	delay1s();
	buzzer_freq_set(10);
	buzzer_start();

   	while (1);
	*/
	
/*
   // ��ʽ����
   double t = 0;

   InitLcd1602();
   LcdShowStr(0, 0, "temp:");  	// 0-4
   LcdShowStr(9, 0, du);

//   ds1302_write_time_hour(12);

   while (1)
   {
   	// ��ȡ�¶Ȳ���ʾ
	   t = Ds18b20ReadTemp();
	   LcdShowDouble(5, 0, t);		// 5-8

   // ��ȡʱ�䲢��ʾ
   		ds1302_read_time_struct();
		LcdShowTimeT(0, 1, mytime);
   }
*/

}