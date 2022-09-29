#include "key.h"



static void delay10ms(void)   //��� 0us
{
    unsigned char a,b,c;
    for(c=5;c>0;c--)
        for(b=4;b>0;b--)
            for(a=248;a>0;a--);
}

// ���øú������ȥ���а���ɨ�裬���û���˰������򷵻�0��������˰�����
// �򷵻ذ��°����ļ�ֵ
unsigned char GetKey(void)
{
	unsigned char hang = 0, lie = 0;
	unsigned char keyvalue = 0;

	 // ��1�غϵ�1��
	 KEY = 0x0f;				// ��IO�������дIO��
	 if (KEY != 0x0f)			// ��IO�����룬��IO��
	 {
	 	 // �����Ĳ���0x0f˵���а���������
		 // ��1�غϵ�2���������˿ڴӶ���ֵ���ж�����һ��
		 
		 delay10ms();
		 // ��һ�غ�������к�
		 switch (KEY)
		 {
		 	case 0x0e:	hang = 1;	break;
			case 0x0d:	hang = 2;	break;
			case 0x0b:	hang = 3;	break;
			case 0x07:	hang = 4;	break;
			default: 				break;
		 }

		 // ��2�غϵ�1��
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

			// ����2���غϺ�hang��lie��֪���ˣ�Ȼ�����hang��lieȥ�����ֵ����
			keyvalue = (hang - 1) * 4 + lie;

			return keyvalue;
		 }
	 }

	 return 0;
}