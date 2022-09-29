#include "ds18b20.h"  
#include <intrins.h>

#include "uart.h"


/******* 延时函数 *******************/
static void delay750us(void)   //误差 -0.217013888889us
{
    unsigned char a,b;
    for(b=198;b>0;b--)
        for(a=2;a>0;a--);
}

static void delay15us(void)   //误差 -0.894097222222us
{
    unsigned char a;
    for(a=4;a>0;a--);
}

static void delay70us(void)   //误差 -0.555555555556us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=28;a>0;a--);
}

static void delay45us(void)   //误差 -0.512152777778us
{
    unsigned char a;
    for(a=18;a>0;a--);
}

static void delay1ms(void)   //误差 -0.651041666667us
{
    unsigned char a,b;
    for(b=4;b>0;b--)
        for(a=113;a>0;a--);
}

static void delay750ms(void)   //误差 -0.000000000183us
{
    unsigned char a,b,c;
    for(c=147;c>0;c--)
        for(b=127;b>0;b--)
            for(a=17;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
}

/****** 低层时序 *********************************************/

// 返回0则表示初始化成功，返回1则表示初始化失败
static unsigned char Ds18b20Init(void)
{
	unsigned char i = 0;

	DATA = 0;			//将总线拉低480us~960us
	delay750us();		// 实际延时750us，符合480-960之间的条件
	DATA = 1;			//然后拉高总线，如果DS18B20做出反应会将在15us~60us后总线拉低

	i = 0;
	while (DATA)	//等待DS18B20拉低总线
	{
		i++;
		if(i>5)//等待>5MS
		{
			return 1;//初始化失败
		}
		delay15us();	
	}
	return 0;//初始化成功
}


static void Ds18b20WriteByte(unsigned char dat)
{
	unsigned int i = 0, j = 0;

	for (j=0; j<8; j++)
	{
		DATA = 0;	     	  	//每写入一位数据之前先把总线拉低1us
		i++;
		DATA = dat & 0x01;  	//然后写入一个数据，从最低位开始
		delay70us();			// 时序要求最少60us
		DATA = 1;				//然后释放总线，至少1us给总线恢复时间才能接着写入第二个数值
		dat >>= 1;
	}
}


static unsigned char Ds18b20ReadByte()
{
	unsigned char byte = 0, bi = 0;
	unsigned int i = 0, j = 0;
		
	for (j=8; j>0; j--)
	{
		DATA = 0;		//先将总线拉低1us
		i++;
		DATA = 1;		//然后释放总线
		i++;
		i++;			//延时6us等待数据稳定
		bi = DATA;	 	//读取数据，从最低位开始读取
		/*将byte左移一位，然后与上右移7位后的bi，注意移动之后移掉那位补0。*/
		byte = (byte >> 1) | (bi << 7);	
		//byte |= (bi << (8-j));
		delay45us();
	}				
	return byte;
}


/*************** 高层时序 *************************************/
static void Ds18b20ChangTemp(void)
{
	Ds18b20Init();
	delay1ms();
	Ds18b20WriteByte(0xcc);		//跳过ROM操作命令		 
	Ds18b20WriteByte(0x44);	    //温度转换命令
	delay750ms();	//等待转换成功，而如果你是一直刷着的话，就不用这个延时了 
}

static void Ds18b20ReadTempCom(void)
{	
	Ds18b20Init();
	delay1ms();
	Ds18b20WriteByte(0xcc);	 //跳过ROM操作命令
	Ds18b20WriteByte(0xbe);	 //发送读取温度命令
}


double Ds18b20ReadTemp(void)
{
	unsigned int temp = 0;
	unsigned char tmh = 0, tml = 0;
	double t = 0;


	Ds18b20ChangTemp();			 	//先写入转换命令
	Ds18b20ReadTempCom();			//然后等待转换完后发送读取温度命令
	tml = Ds18b20ReadByte();		//读取温度值共16位，先读低字节
	tmh = Ds18b20ReadByte();		//再读高字节
//	temp = tmh;
//	temp <<= 8;
//	temp |= tml;
	temp = tml | (tmh << 8);

	t = temp * 0.0625;

	return t;
}
 





