#include "1602.h"



// ��LCD1602�ĵײ��Լ��߲�ʱ������װ




/************ �Ͳ�ʱ�� ********************************/
static void Read_Busy()           //æ��⺯�����ж�bit7��0������ִ�У�1��ֹ
{
    unsigned char sta;      //
    LCD1602_DB = 0xff;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do
    {
        LCD1602_EN = 1;
        sta = LCD1602_DB;
        LCD1602_EN = 0;    //ʹ�ܣ���������ͣ��ͷ�����
    }while(sta & 0x80);
}

static void Lcd1602_Write_Cmd(unsigned char cmd)     //д����
{
    Read_Busy();
    LCD1602_RS = 0;
    LCD1602_RW = 0;	
    LCD1602_DB = cmd;
    LCD1602_EN = 1;
    LCD1602_EN = 0;    
}

static void Lcd1602_Write_Data(unsigned char dat)   //д����
{
      Read_Busy();
      LCD1602_RS = 1;
      LCD1602_RW = 0;
      LCD1602_DB = dat;
      LCD1602_EN = 1;
      LCD1602_EN = 0;
}

/************* �߲�ʱ�� ******************************/
// �������������õ�ǰ���λ�ã���ʵ�������õ�ǰ���ڱ༭��λ�ã�
// ��ʵ�����ڲ������ݵ�ַָ�룬��ʵ����RAM�Դ��ƫ����
// x��Χ��0-15��y=0��ʾ����һ�У�y=1��ʾ����һ��
void LcdSetCursor(unsigned char x,unsigned char y)  //������ʾ
{
    unsigned char addr;
    if(y == 0)
        addr = 0x00 + x;
    else
        addr = 0x40 + x;
    
    Lcd1602_Write_Cmd(addr|0x80);
}

// ���������ǣ�������(x,y)��ʼ��ʾ�ַ���str
// ע������������ܿ�����ʾ����Ϊ�Դ��ַ�ǲ�������
// ��ʵ���ǿ��Է�װ��һ���ܹ�������ʾ�ĺ�����
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str)     //��ʾ�ַ���
{
    LcdSetCursor(x,y);      //��ǰ�ַ�������
    while(*str != '\0')
    {
        Lcd1602_Write_Data(*str++);
    }
}

// ��ʼ��LCD��ʹ֮�ܹ���ʼ��������
void InitLcd1602(void)              //1602��ʼ��
{
    Lcd1602_Write_Cmd(0x38);    //�򿪣�5*8,8λ����
    Lcd1602_Write_Cmd(0x0c);	// ����ʾ�����޹��
//	Lcd1602_Write_Cmd(0x0f);	// ����ʾ���ҹ����˸
    Lcd1602_Write_Cmd(0x06);
    Lcd1602_Write_Cmd(0x01);    //����   
}

// ��ʾ������24.5���ֵ�double���͵�����
void LcdShowDouble(unsigned char x, unsigned char y, double d)     
{
	// ��һ������double dת���ַ���str
	unsigned char str[7] = "";
	memset(str, 0, sizeof(str));
	sprintf(str, "%5.2f", d);
	
	// uart
	printf("LcdShowDouble: %f\r\n", d);
	// �ڶ�������ʾstr
	LcdShowStr(x, y, str);
}

void LcdShowTime(struct time_t mytime)
{
	unsigned char str[17] = "";
	printf("%4d%02d%02d-%02d%02d%02d-%d\r\n", mytime.year, mytime.mon, mytime.date, mytime.hour, mytime.min, mytime.sec, mytime.day);
	
	sprintf(str, "%4d%02d%02d-%02d%02d%02d-%d", mytime.year, mytime.mon, mytime.date, mytime.hour, mytime.min, mytime.sec, mytime.day);
	LcdShowStr(0, 0, str);
	
	printf("LcdShowTime: %s\r\n", str);
}	

// ��LCD����ʾһ��1-16������
void LcdShowInt(unsigned char x, unsigned char y, unsigned char num)
{
	unsigned char str[3] = {0};
	sprintf(str, "%d", num);
	LcdShowStr(x, y, str);
}



// ����LCD����ͨģʽ������ʾ��꣩
void LcdNormalMode(void)
{
	Lcd1602_Write_Cmd(0x0C);   	// 00001100
}

// ����LCD������ģʽ����ʾ��꣬���ҹ����˸��
void LcdAjustMode(void)
{
	Lcd1602_Write_Cmd(0x0f);
}