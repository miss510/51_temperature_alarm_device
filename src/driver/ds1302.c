#include "ds1302.h"


/**************  全局变量定义  *************************************/

// 因为51单片机的设计本身RAM比较少而Flash稍微多一些，像这里定义的数组内部
// 的内容是不会变的（常量数组），我们就可以使用code关键字，让编译器帮我们
// 把这个数组放在flash中而不是ram中，这样做可以省一些ram。
//unsigned char code READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
//unsigned char code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};
//unsigned char time[7];		// 用来存储读取的时间的，格式是：秒分时日月周年

// 定义了一个全局变量mytime，mytime是time_t类型的，所以mytime是一个结构体
// 变量，代表一个具体的时间，要占一定的内存。
struct time_t mytime;


// 有用函数
static void delay(void)
{
	unsigned char i;
	for (i=0; i<3; i++);
}


// 向ds1302的内部寄存器addr写入一个值value
static void ds1302_write_reg(unsigned char addr, unsigned char value)
{
	unsigned char i = 0;
	unsigned char dat = 0;

	// 第1部分: 时序起始
	SCLK = 0;
	delay();
	RST = 0;
	delay();
	RST = 1;  		// SCLK为低时，RST由低变高，意味着一个大的周期的开始
	delay();
	// 第2部分: 写入第1字节，addr
	for (i=0; i<8; i++)
	{
		dat = addr & 0x01;	 	// SPI是从低位开始传输的
		DSIO = dat;	 			// 把要发送的bit数据丢到IO引脚上去准备好
		SCLK = 1;		 		// 制造上升沿，让DS1302把IO上的值读走
		delay();				// 读走之后，一个小周期就完了
		SCLK = 0;				// 把SCLK拉低，是为了给下一个小周期做准备
		delay();
		addr >>= 1;	   			// 把addr右移一位
	}
	// 第3部分: 写入第2字节，value
	for (i=0; i<8; i++)
	{
		dat = value & 0x01;	 	// SPI是从低位开始传输的
		DSIO = dat;	 			// 把要发送的bit数据丢到IO引脚上去准备好
		SCLK = 1;		 		// 制造上升沿，让DS1302把IO上的值读走
		delay();				// 读走之后，一个小周期就完了
		SCLK = 0;				// 把SCLK拉低，是为了给下一个小周期做准备
		delay();
		value = value >> 1;	   	// 把addr右移一位
	}
	// 第4部分: 时序结束
	SCLK = 0;				  	// SCLK拉低为了后面的周期时初始状态是对的
	delay();
	RST = 0;					// RST拉低意味着一个大周期的结束
	delay();
}


// 从ds1302的内部寄存器addr读出一个值，作为返回值
static unsigned char ds1302_read_reg(unsigned char addr)
{
	unsigned char i = 0;
	unsigned char dat = 0; 		// 用来存储读取到的一字节数据的
	unsigned char tmp = 0;

	// 第1部分: 时序起始
	SCLK = 0;
	delay();
	RST = 0;
	delay();
	RST = 1;  		// SCLK为低时，RST由低变高，意味着一个大的周期的开始
	delay();
	// 第2部分: 写入要读取的寄存器地址，addr
	for (i=0; i<8; i++)
	{
		dat = addr & 0x01;	 	// SPI是从低位开始传输的
		DSIO = dat;	 			// 把要发送的bit数据丢到IO引脚上去准备好
		SCLK = 1;		 		// 制造上升沿，让DS1302把IO上的值读走
		delay();				// 读走之后，一个小周期就完了
		SCLK = 0;				// 把SCLK拉低，是为了给下一个小周期做准备
		delay();
		addr >>= 1;	   			// 把addr右移一位
	}
	// 第3部分: 读出一字节DS1302返回给我们的值
	dat = 0;
	for (i=0; i<8; i++)
	{
	// 在前面向ds1302写入addr的最后一个bit后，ds1302就会将读取到的寄存器值
	// 的第一个bit放入到IO引脚上，所以我们应该先读取IO再制造下降沿然后继续
	// 读取下一个bit
		tmp = DSIO;
		dat |= (tmp << i);		// 读出来的数值是低位在前的
		SCLK = 1;  				// 由于上面SCLK是低，所以要先拉到高
		delay();
		SCLK = 0;				// 拉低SCLK制造一个下降沿
		delay();
	}
	// 第4部分: 时序结束
	SCLK = 0;				  	// SCLK拉低为了后面的周期时初始状态是对的
	delay();
	RST = 0;					// RST拉低意味着一个大周期的结束
	delay();

	// 第5部分：解决读取时间是ff的问题
	DSIO = 0;

	return dat;
}

/*
void ds1302_read_time(void)
{
	unsigned char i = 0;
	for (i=0; i<7; i++)
	{
		time[i] = ds1302_read_reg(READ_RTC_ADDR[i]);
	}
}
*/

// 实现2个子函数，分别实现从bcd码转十六进制，和十六进制转bcd码
unsigned char bcd2hex(unsigned char bcd)
{
	// 譬如我们现在要把bcd码0x24转成24（0x18）
	// 思路就是分2步
	// 第1步，先从0x24得到2和4
	// ((bcd & 0xf0) >> 4) 高4位，也就是2
	// (bcd & 0x0f) 低4位，也就是4
	// 第2步，由2*10+4得到24
	return (((bcd & 0xf0) >> 4) * 10 + (bcd & 0x0f));
}

unsigned char hex2bcd(unsigned char hex)
{
  	// 就是要把24转成0x24
	// 第一步，先由24得到2和4
	// (24 / 10) 就是2， (24 % 10)就是4
	// 第二步，再组合成0x24
	return (((hex / 10) << 4) | (hex % 10));
}



// 用结构体方式来实现的读取时间的函数
// READ_RTC_ADDR格式是：秒分时日月周年
void ds1302_read_time_struct(void)
{
	mytime.year =  bcd2hex(ds1302_read_reg(REG_ADDR_YEAR_READ)) + 2000;
   	mytime.mon 	=  bcd2hex(ds1302_read_reg(REG_ADDR_MON_READ));
	mytime.date =  bcd2hex(ds1302_read_reg(REG_ADDR_DATE_READ));
	mytime.hour =  bcd2hex(ds1302_read_reg(REG_ADDR_HOUR_READ));
	mytime.min 	=  bcd2hex(ds1302_read_reg(REG_ADDR_MIN_READ));
	mytime.sec 	=  bcd2hex(ds1302_read_reg(REG_ADDR_SEC_READ));
	mytime.day 	=  bcd2hex(ds1302_read_reg(REG_ADDR_DAY_READ));
}

// 本函数用于向DS1302中写入一个时间t1
void ds1302_write_time_struct(struct time_t t1)
{
	ds1302_write_reg(0x8E, 0x00);	// 去掉写保护
	// 依次写各个时间寄存器
	ds1302_write_reg(REG_ADDR_YEAR_WRITE, (hex2bcd(t1.year - 2000)));
	ds1302_write_reg(REG_ADDR_MON_WRITE, (hex2bcd(t1.mon)));
	ds1302_write_reg(REG_ADDR_DATE_WRITE, (hex2bcd(t1.date)));
	ds1302_write_reg(REG_ADDR_HOUR_WRITE, (hex2bcd(t1.hour)));
	ds1302_write_reg(REG_ADDR_MIN_WRITE, (hex2bcd(t1.min)));
	ds1302_write_reg(REG_ADDR_SEC_WRITE, (hex2bcd(t1.sec)));
	ds1302_write_reg(REG_ADDR_DAY_WRITE, (hex2bcd(t1.day)));

	ds1302_write_reg(0x8E, 0x80);	// 打开写保护
}

// 只修改年份
void ds1302_write_time_year(unsigned int year)
{
	ds1302_write_reg(0x8E, 0x00);	// 去掉写保护
	// 依次写各个时间寄存器
	ds1302_write_reg(REG_ADDR_YEAR_WRITE, (hex2bcd(year - 2000)));

	ds1302_write_reg(0x8E, 0x80);	// 打开写保护
}

// 只修改月份
void ds1302_write_time_mon(unsigned char mon)
{
	ds1302_write_reg(0x8E, 0x00);	// 去掉写保护
	// 依次写各个时间寄存器
	ds1302_write_reg(REG_ADDR_MON_WRITE, (hex2bcd(mon)));

	ds1302_write_reg(0x8E, 0x80);	// 打开写保护
}

// 只修改日
void ds1302_write_time_date(unsigned char date)
{
	ds1302_write_reg(0x8E, 0x00);	// 去掉写保护
	// 依次写各个时间寄存器
	ds1302_write_reg(REG_ADDR_DATE_WRITE, (hex2bcd(date)));

	ds1302_write_reg(0x8E, 0x80);	// 打开写保护
}

// 只修改小时数
void ds1302_write_time_hour(unsigned char hour)
{
	ds1302_write_reg(0x8E, 0x00);	// 去掉写保护
	// 依次写各个时间寄存器
	ds1302_write_reg(REG_ADDR_HOUR_WRITE, (hex2bcd(hour)));

	ds1302_write_reg(0x8E, 0x80);	// 打开写保护
}

// 只修改分钟数
void ds1302_write_time_min(unsigned char min)
{
	ds1302_write_reg(0x8E, 0x00);	// 去掉写保护
	// 依次写各个时间寄存器
	ds1302_write_reg(REG_ADDR_MIN_WRITE, (hex2bcd(min)));

	ds1302_write_reg(0x8E, 0x80);	// 打开写保护
}

// 只修改秒数
void ds1302_write_time_sec(unsigned char sec)
{
	ds1302_write_reg(0x8E, 0x00);	// 去掉写保护
	// 依次写各个时间寄存器
	ds1302_write_reg(REG_ADDR_SEC_WRITE, (hex2bcd(sec)));

	ds1302_write_reg(0x8E, 0x80);	// 打开写保护
}


/*
void ds1302_write_time(void)
{
	unsigned char i = 0;

	// 准备好要写入的时间
	time[0] = 0x24;				// 对应 24s
	time[1] = 0x39;				// 对应 39m
	time[2] = 0x11;				// 对应 11h
	time[3] = 0x06;				// 对应 6日
	time[4] = 0x12;				// 对应 12月
	time[5] = 0x02;				// 对应 星期2
	time[6] = 0x16;				// 对应 2016年

	ds1302_write_reg(0x8E, 0x00);	// 去掉写保护
	for (i=0; i<7; i++)
	{
		ds1302_write_reg(WRITE_RTC_ADDR[i], time[i]);
	}
	ds1302_write_reg(0x8E, 0x80);	// 打开写保护
}
*/


/*
// 通过串口将7个时间以二进制方式输出在串口助手上
void debug_print_time(void)
{
	unsigned char i = 0;

	while (1)
	{
		// 1 从DS1302读取时间
		ds1302_read_time();

		// 2 for循环内打印一组7个时间
		for (i=0; i<7; i++)
		{
			uart_send_byte(time[i]);	
		}

		// 3 延时900ms后再继续下个周期
	   	delay900ms();
	}	
}
*/














