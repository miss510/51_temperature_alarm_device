#include "key.h"



static void delay10ms(void)   //误差 0us
{
    unsigned char a,b,c;
    for(c=5;c>0;c--)
        for(b=4;b>0;b--)
            for(a=248;a>0;a--);
}

// 调用该函数后会去进行按键扫描，如果没有人按按键则返回0，如果有人按按键
// 则返回按下按键的键值
unsigned char GetKey(void)
{
	unsigned char hang = 0, lie = 0;
	unsigned char keyvalue = 0;

	 // 第1回合第1步
	 KEY = 0x0f;				// 从IO口输出，写IO口
	 if (KEY != 0x0f)			// 从IO口输入，读IO口
	 {
	 	 // 读出的不是0x0f说明有按键被按下
		 // 第1回合第2步：读出端口从读出值来判断是哪一行
		 
		 delay10ms();
		 // 第一回合中算出行号
		 switch (KEY)
		 {
		 	case 0x0e:	hang = 1;	break;
			case 0x0d:	hang = 2;	break;
			case 0x0b:	hang = 3;	break;
			case 0x07:	hang = 4;	break;
			default: 				break;
		 }

		 // 第2回合第1步
		 KEY = 0xf0;
		 if (KEY != 0xf0)
		 {
		 	  switch (KEY)
			 {
			 	case 0xe0:	lie = 1;	break;
				case 0xd0:	lie = 2;	break;
				case 0xb0:	lie = 3;	break;
				case 0x70:	lie = 4;	break;
				default: 				break;
			 }

			// 经过2个回合后hang和lie都知道了，然后根据hang和lie去计算键值即可
			keyvalue = (hang - 1) * 4 + lie;

			return keyvalue;
		 }
	 }

	 return 0;
}