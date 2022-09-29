#include "ds18b20.h"  
#include <intrins.h>

#include "uart.h"


/******* ��ʱ���� *******************/
static void delay750us(void)   //��� -0.217013888889us
{
    unsigned char a,b;
    for(b=198;b>0;b--)
        for(a=2;a>0;a--);
}

static void delay15us(void)   //��� -0.894097222222us
{
    unsigned char a;
    for(a=4;a>0;a--);
}

static void delay70us(void)   //��� -0.555555555556us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=28;a>0;a--);
}

static void delay45us(void)   //��� -0.512152777778us
{
    unsigned char a;
    for(a=18;a>0;a--);
}

static void delay1ms(void)   //��� -0.651041666667us
{
    unsigned char a,b;
    for(b=4;b>0;b--)
        for(a=113;a>0;a--);
}

static void delay750ms(void)   //��� -0.000000000183us
{
    unsigned char a,b,c;
    for(c=147;c>0;c--)
        for(b=127;b>0;b--)
            for(a=17;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
}

/****** �Ͳ�ʱ�� *********************************************/

// ����0���ʾ��ʼ���ɹ�������1���ʾ��ʼ��ʧ��
static unsigned char Ds18b20Init(void)
{
	unsigned char i = 0;

	DATA = 0;			//����������480us~960us
	delay750us();		// ʵ����ʱ750us������480-960֮�������
	DATA = 1;			//Ȼ���������ߣ����DS18B20������Ӧ�Ὣ��15us~60us����������

	i = 0;
	while (DATA)	//�ȴ�DS18B20��������
	{
		i++;
		if(i>5)//�ȴ�>5MS
		{
			return 1;//��ʼ��ʧ��
		}
		delay15us();	
	}
	return 0;//��ʼ���ɹ�
}


static void Ds18b20WriteByte(unsigned char dat)
{
	unsigned int i = 0, j = 0;

	for (j=0; j<8; j++)
	{
		DATA = 0;	     	  	//ÿд��һλ����֮ǰ�Ȱ���������1us
		i++;
		DATA = dat & 0x01;  	//Ȼ��д��һ�����ݣ������λ��ʼ
		delay70us();			// ʱ��Ҫ������60us
		DATA = 1;				//Ȼ���ͷ����ߣ�����1us�����߻ָ�ʱ����ܽ���д��ڶ�����ֵ
		dat >>= 1;
	}
}


static unsigned char Ds18b20ReadByte()
{
	unsigned char byte = 0, bi = 0;
	unsigned int i = 0, j = 0;
		
	for (j=8; j>0; j--)
	{
		DATA = 0;		//�Ƚ���������1us
		i++;
		DATA = 1;		//Ȼ���ͷ�����
		i++;
		i++;			//��ʱ6us�ȴ������ȶ�
		bi = DATA;	 	//��ȡ���ݣ������λ��ʼ��ȡ
		/*��byte����һλ��Ȼ����������7λ���bi��ע���ƶ�֮���Ƶ���λ��0��*/
		byte = (byte >> 1) | (bi << 7);	
		//byte |= (bi << (8-j));
		delay45us();
	}				
	return byte;
}


/*************** �߲�ʱ�� *************************************/
static void Ds18b20ChangTemp(void)
{
	Ds18b20Init();
	delay1ms();
	Ds18b20WriteByte(0xcc);		//����ROM��������		 
	Ds18b20WriteByte(0x44);	    //�¶�ת������
	delay750ms();	//�ȴ�ת���ɹ������������һֱˢ�ŵĻ����Ͳ��������ʱ�� 
}

static void Ds18b20ReadTempCom(void)
{	
	Ds18b20Init();
	delay1ms();
	Ds18b20WriteByte(0xcc);	 //����ROM��������
	Ds18b20WriteByte(0xbe);	 //���Ͷ�ȡ�¶�����
}


double Ds18b20ReadTemp(void)
{
	unsigned int temp = 0;
	unsigned char tmh = 0, tml = 0;
	double t = 0;


	Ds18b20ChangTemp();			 	//��д��ת������
	Ds18b20ReadTempCom();			//Ȼ��ȴ�ת������Ͷ�ȡ�¶�����
	tml = Ds18b20ReadByte();		//��ȡ�¶�ֵ��16λ���ȶ����ֽ�
	tmh = Ds18b20ReadByte();		//�ٶ����ֽ�
//	temp = tmh;
//	temp <<= 8;
//	temp |= tml;
	temp = tml | (tmh << 8);

	t = temp * 0.0625;

	return t;
}
 





