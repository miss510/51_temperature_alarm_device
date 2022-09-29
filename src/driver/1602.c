#include "1602.h"



// 对LCD1602的底层以及高层时序做封装




/************ 低层时序 ********************************/
static void Read_Busy()           //忙检测函数，判断bit7是0，允许执行；1禁止
{
    unsigned char sta;      //
    LCD1602_DB = 0xff;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do
    {
        LCD1602_EN = 1;
        sta = LCD1602_DB;
        LCD1602_EN = 0;    //使能，用完就拉低，释放总线
    }while(sta & 0x80);
}

static void Lcd1602_Write_Cmd(unsigned char cmd)     //写命令
{
    Read_Busy();
    LCD1602_RS = 0;
    LCD1602_RW = 0;	
    LCD1602_DB = cmd;
    LCD1602_EN = 1;
    LCD1602_EN = 0;    
}

static void Lcd1602_Write_Data(unsigned char dat)   //写数据
{
      Read_Busy();
      LCD1602_RS = 1;
      LCD1602_RW = 0;
      LCD1602_DB = dat;
      LCD1602_EN = 1;
      LCD1602_EN = 0;
}

/************* 高层时序 ******************************/
// 本函数用来设置当前光标位置，其实就是设置当前正在编辑的位置，
// 其实就是内部的数据地址指针，其实就是RAM显存的偏移量
// x范围是0-15，y=0表示上面一行，y=1表示下面一行
void LcdSetCursor(unsigned char x,unsigned char y)  //坐标显示
{
    unsigned char addr;
    if(y == 0)
        addr = 0x00 + x;
    else
        addr = 0x40 + x;
    
    Lcd1602_Write_Cmd(addr|0x80);
}

// 函数功能是：从坐标(x,y)开始显示字符串str
// 注意这个函数不能跨行显示，因为显存地址是不连续的
// 其实我们可以封装出一个能够折行显示的函数的
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str)     //显示字符串
{
    LcdSetCursor(x,y);      //当前字符的坐标
    while(*str != '\0')
    {
        Lcd1602_Write_Data(*str++);
    }
}

// 初始化LCD，使之能够开始正常工作
void InitLcd1602(void)              //1602初始化
{
    Lcd1602_Write_Cmd(0x38);    //打开，5*8,8位数据
    Lcd1602_Write_Cmd(0x0c);	// 打开显示并且无光标
//	Lcd1602_Write_Cmd(0x0f);	// 打开显示并且光标闪烁
    Lcd1602_Write_Cmd(0x06);
    Lcd1602_Write_Cmd(0x01);    //清屏   
}

// 显示类似于24.5这种的double类型的数字
void LcdShowDouble(unsigned char x, unsigned char y, double d)     
{
	// 第一步：将double d转成字符串str
	unsigned char str[7] = "";
	memset(str, 0, sizeof(str));
	sprintf(str, "%5.2f", d);
	
	// uart
	printf("LcdShowDouble: %f\r\n", d);
	// 第二步：显示str
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

// 在LCD上显示一个1-16的数字
void LcdShowInt(unsigned char x, unsigned char y, unsigned char num)
{
	unsigned char str[3] = {0};
	sprintf(str, "%d", num);
	LcdShowStr(x, y, str);
}



// 设置LCD到普通模式（不显示光标）
void LcdNormalMode(void)
{
	Lcd1602_Write_Cmd(0x0C);   	// 00001100
}

// 设置LCD到调整模式（显示光标，并且光标闪烁）
void LcdAjustMode(void)
{
	Lcd1602_Write_Cmd(0x0f);
}