#include "ds1302.h"


/**************  ȫ�ֱ�������  *************************************/

// ��Ϊ51��Ƭ������Ʊ���RAM�Ƚ��ٶ�Flash��΢��һЩ�������ﶨ��������ڲ�
// �������ǲ����ģ��������飩�����ǾͿ���ʹ��code�ؼ��֣��ñ�����������
// ������������flash�ж�����ram�У�����������ʡһЩram��
//unsigned char code READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
//unsigned char code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};
//unsigned char time[7];		// �����洢��ȡ��ʱ��ģ���ʽ�ǣ����ʱ��������

// ������һ��ȫ�ֱ���mytime��mytime��time_t���͵ģ�����mytime��һ���ṹ��
// ����������һ�������ʱ�䣬Ҫռһ�����ڴ档
struct time_t mytime;


// ���ú���
static void delay(void)
{
	unsigned char i;
	for (i=0; i<3; i++);
}


// ��ds1302���ڲ��Ĵ���addrд��һ��ֵvalue
static void ds1302_write_reg(unsigned char addr, unsigned char value)
{
	unsigned char i = 0;
	unsigned char dat = 0;

	// ��1����: ʱ����ʼ
	SCLK = 0;
	delay();
	RST = 0;
	delay();
	RST = 1;  		// SCLKΪ��ʱ��RST�ɵͱ�ߣ���ζ��һ��������ڵĿ�ʼ
	delay();
	// ��2����: д���1�ֽڣ�addr
	for (i=0; i<8; i++)
	{
		dat = addr & 0x01;	 	// SPI�Ǵӵ�λ��ʼ�����
		DSIO = dat;	 			// ��Ҫ���͵�bit���ݶ���IO������ȥ׼����
		SCLK = 1;		 		// ���������أ���DS1302��IO�ϵ�ֵ����
		delay();				// ����֮��һ��С���ھ�����
		SCLK = 0;				// ��SCLK���ͣ���Ϊ�˸���һ��С������׼��
		delay();
		addr >>= 1;	   			// ��addr����һλ
	}
	// ��3����: д���2�ֽڣ�value
	for (i=0; i<8; i++)
	{
		dat = value & 0x01;	 	// SPI�Ǵӵ�λ��ʼ�����
		DSIO = dat;	 			// ��Ҫ���͵�bit���ݶ���IO������ȥ׼����
		SCLK = 1;		 		// ���������أ���DS1302��IO�ϵ�ֵ����
		delay();				// ����֮��һ��С���ھ�����
		SCLK = 0;				// ��SCLK���ͣ���Ϊ�˸���һ��С������׼��
		delay();
		value = value >> 1;	   	// ��addr����һλ
	}
	// ��4����: ʱ�����
	SCLK = 0;				  	// SCLK����Ϊ�˺��������ʱ��ʼ״̬�ǶԵ�
	delay();
	RST = 0;					// RST������ζ��һ�������ڵĽ���
	delay();
}


// ��ds1302���ڲ��Ĵ���addr����һ��ֵ����Ϊ����ֵ
static unsigned char ds1302_read_reg(unsigned char addr)
{
	unsigned char i = 0;
	unsigned char dat = 0; 		// �����洢��ȡ����һ�ֽ����ݵ�
	unsigned char tmp = 0;

	// ��1����: ʱ����ʼ
	SCLK = 0;
	delay();
	RST = 0;
	delay();
	RST = 1;  		// SCLKΪ��ʱ��RST�ɵͱ�ߣ���ζ��һ��������ڵĿ�ʼ
	delay();
	// ��2����: д��Ҫ��ȡ�ļĴ�����ַ��addr
	for (i=0; i<8; i++)
	{
		dat = addr & 0x01;	 	// SPI�Ǵӵ�λ��ʼ�����
		DSIO = dat;	 			// ��Ҫ���͵�bit���ݶ���IO������ȥ׼����
		SCLK = 1;		 		// ���������أ���DS1302��IO�ϵ�ֵ����
		delay();				// ����֮��һ��С���ھ�����
		SCLK = 0;				// ��SCLK���ͣ���Ϊ�˸���һ��С������׼��
		delay();
		addr >>= 1;	   			// ��addr����һλ
	}
	// ��3����: ����һ�ֽ�DS1302���ظ����ǵ�ֵ
	dat = 0;
	for (i=0; i<8; i++)
	{
	// ��ǰ����ds1302д��addr�����һ��bit��ds1302�ͻὫ��ȡ���ļĴ���ֵ
	// �ĵ�һ��bit���뵽IO�����ϣ���������Ӧ���ȶ�ȡIO�������½���Ȼ�����
	// ��ȡ��һ��bit
		tmp = DSIO;
		dat |= (tmp << i);		// ����������ֵ�ǵ�λ��ǰ��
		SCLK = 1;  				// ��������SCLK�ǵͣ�����Ҫ��������
		delay();
		SCLK = 0;				// ����SCLK����һ���½���
		delay();
	}
	// ��4����: ʱ�����
	SCLK = 0;				  	// SCLK����Ϊ�˺��������ʱ��ʼ״̬�ǶԵ�
	delay();
	RST = 0;					// RST������ζ��һ�������ڵĽ���
	delay();

	// ��5���֣������ȡʱ����ff������
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

// ʵ��2���Ӻ������ֱ�ʵ�ִ�bcd��תʮ�����ƣ���ʮ������תbcd��
unsigned char bcd2hex(unsigned char bcd)
{
	// Ʃ����������Ҫ��bcd��0x24ת��24��0x18��
	// ˼·���Ƿ�2��
	// ��1�����ȴ�0x24�õ�2��4
	// ((bcd & 0xf0) >> 4) ��4λ��Ҳ����2
	// (bcd & 0x0f) ��4λ��Ҳ����4
	// ��2������2*10+4�õ�24
	return (((bcd & 0xf0) >> 4) * 10 + (bcd & 0x0f));
}

unsigned char hex2bcd(unsigned char hex)
{
  	// ����Ҫ��24ת��0x24
	// ��һ��������24�õ�2��4
	// (24 / 10) ����2�� (24 % 10)����4
	// �ڶ���������ϳ�0x24
	return (((hex / 10) << 4) | (hex % 10));
}



// �ýṹ�巽ʽ��ʵ�ֵĶ�ȡʱ��ĺ���
// READ_RTC_ADDR��ʽ�ǣ����ʱ��������
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

// ������������DS1302��д��һ��ʱ��t1
void ds1302_write_time_struct(struct time_t t1)
{
	ds1302_write_reg(0x8E, 0x00);	// ȥ��д����
	// ����д����ʱ��Ĵ���
	ds1302_write_reg(REG_ADDR_YEAR_WRITE, (hex2bcd(t1.year - 2000)));
	ds1302_write_reg(REG_ADDR_MON_WRITE, (hex2bcd(t1.mon)));
	ds1302_write_reg(REG_ADDR_DATE_WRITE, (hex2bcd(t1.date)));
	ds1302_write_reg(REG_ADDR_HOUR_WRITE, (hex2bcd(t1.hour)));
	ds1302_write_reg(REG_ADDR_MIN_WRITE, (hex2bcd(t1.min)));
	ds1302_write_reg(REG_ADDR_SEC_WRITE, (hex2bcd(t1.sec)));
	ds1302_write_reg(REG_ADDR_DAY_WRITE, (hex2bcd(t1.day)));

	ds1302_write_reg(0x8E, 0x80);	// ��д����
}

// ֻ�޸����
void ds1302_write_time_year(unsigned int year)
{
	ds1302_write_reg(0x8E, 0x00);	// ȥ��д����
	// ����д����ʱ��Ĵ���
	ds1302_write_reg(REG_ADDR_YEAR_WRITE, (hex2bcd(year - 2000)));

	ds1302_write_reg(0x8E, 0x80);	// ��д����
}

// ֻ�޸��·�
void ds1302_write_time_mon(unsigned char mon)
{
	ds1302_write_reg(0x8E, 0x00);	// ȥ��д����
	// ����д����ʱ��Ĵ���
	ds1302_write_reg(REG_ADDR_MON_WRITE, (hex2bcd(mon)));

	ds1302_write_reg(0x8E, 0x80);	// ��д����
}

// ֻ�޸���
void ds1302_write_time_date(unsigned char date)
{
	ds1302_write_reg(0x8E, 0x00);	// ȥ��д����
	// ����д����ʱ��Ĵ���
	ds1302_write_reg(REG_ADDR_DATE_WRITE, (hex2bcd(date)));

	ds1302_write_reg(0x8E, 0x80);	// ��д����
}

// ֻ�޸�Сʱ��
void ds1302_write_time_hour(unsigned char hour)
{
	ds1302_write_reg(0x8E, 0x00);	// ȥ��д����
	// ����д����ʱ��Ĵ���
	ds1302_write_reg(REG_ADDR_HOUR_WRITE, (hex2bcd(hour)));

	ds1302_write_reg(0x8E, 0x80);	// ��д����
}

// ֻ�޸ķ�����
void ds1302_write_time_min(unsigned char min)
{
	ds1302_write_reg(0x8E, 0x00);	// ȥ��д����
	// ����д����ʱ��Ĵ���
	ds1302_write_reg(REG_ADDR_MIN_WRITE, (hex2bcd(min)));

	ds1302_write_reg(0x8E, 0x80);	// ��д����
}

// ֻ�޸�����
void ds1302_write_time_sec(unsigned char sec)
{
	ds1302_write_reg(0x8E, 0x00);	// ȥ��д����
	// ����д����ʱ��Ĵ���
	ds1302_write_reg(REG_ADDR_SEC_WRITE, (hex2bcd(sec)));

	ds1302_write_reg(0x8E, 0x80);	// ��д����
}


/*
void ds1302_write_time(void)
{
	unsigned char i = 0;

	// ׼����Ҫд���ʱ��
	time[0] = 0x24;				// ��Ӧ 24s
	time[1] = 0x39;				// ��Ӧ 39m
	time[2] = 0x11;				// ��Ӧ 11h
	time[3] = 0x06;				// ��Ӧ 6��
	time[4] = 0x12;				// ��Ӧ 12��
	time[5] = 0x02;				// ��Ӧ ����2
	time[6] = 0x16;				// ��Ӧ 2016��

	ds1302_write_reg(0x8E, 0x00);	// ȥ��д����
	for (i=0; i<7; i++)
	{
		ds1302_write_reg(WRITE_RTC_ADDR[i], time[i]);
	}
	ds1302_write_reg(0x8E, 0x80);	// ��д����
}
*/


/*
// ͨ�����ڽ�7��ʱ���Զ����Ʒ�ʽ����ڴ���������
void debug_print_time(void)
{
	unsigned char i = 0;

	while (1)
	{
		// 1 ��DS1302��ȡʱ��
		ds1302_read_time();

		// 2 forѭ���ڴ�ӡһ��7��ʱ��
		for (i=0; i<7; i++)
		{
			uart_send_byte(time[i]);	
		}

		// 3 ��ʱ900ms���ټ����¸�����
	   	delay900ms();
	}	
}
*/














