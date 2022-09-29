#ifndef __DS1302_H__
#define __DS1302_H__

#include <reg51.h>
#include <intrins.h>

// 用宏定义的方式来定义时间的寄存器地址 格式是：秒分时日月周年
#define REG_ADDR_YEAR_WRITE	 	0x8c
#define REG_ADDR_MON_WRITE	 	0x88
#define REG_ADDR_DATE_WRITE	 	0x86
#define REG_ADDR_HOUR_WRITE	 	0x84
#define REG_ADDR_MIN_WRITE	 	0x82
#define REG_ADDR_SEC_WRITE	 	0x80
#define REG_ADDR_DAY_WRITE	 	0x8a

#define REG_ADDR_YEAR_READ	 	(REG_ADDR_YEAR_WRITE+1)
#define REG_ADDR_MON_READ	 	(REG_ADDR_MON_WRITE+1)
#define REG_ADDR_DATE_READ	 	(REG_ADDR_DATE_WRITE+1)
#define REG_ADDR_HOUR_READ	 	(REG_ADDR_HOUR_WRITE+1)
#define REG_ADDR_MIN_READ	 	(REG_ADDR_MIN_WRITE+1)
#define REG_ADDR_SEC_READ	 	(REG_ADDR_SEC_WRITE+1)
#define REG_ADDR_DAY_READ	 	(REG_ADDR_DAY_WRITE+1)

// 定义SPI的三根引脚
sbit DSIO	= P3^4;
sbit RST	= P3^5;
sbit SCLK	= P3^6;

// 封装出来的一个表示时间的结构体类型
// 类型不占内存的，也不表示一个具体时间，但是类型可以用来生成时间变量
// 每一个时间变量就占一定内存，每一个时间变量就代表一个具体的时间
struct time_t
{
   unsigned int 	year;  	// 2017
   unsigned char 	mon;	// 1-12
   unsigned char 	date;	// 1-31
   unsigned	char 	hour;	// 0-23
   unsigned char 	min;	// 0-59
   unsigned char	sec;	// 0-59
   unsigned char 	day;	// 0-6
};


// 全局变量声明
extern struct time_t mytime;

// 外部函数声明
void ds1302_read_time_struct(void);
void ds1302_write_time_struct(struct time_t t1);
void ds1302_write_time_year(unsigned int year);
void ds1302_write_time_mon(unsigned char mon);
void ds1302_write_time_date(unsigned char date);
void ds1302_write_time_hour(unsigned char hour);
void ds1302_write_time_min(unsigned char min);
void ds1302_write_time_sec(unsigned char sec);


#endif





