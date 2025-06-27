#include "base.h"
#include "segled.h"//引用seg_code
//#include "segcode.h"

void SEG_1LED(unsigned char pos, unsigned char segCode)
{
	unsigned int i;
	//设置段选全灭，否则其他位会出现残影
	SelectChannel(Channel_SegLED_Segment);
	P0 = 0xff;//共阳全灭
	//设置位选信号，定位到单个数码管，数码管从左到右为0-7
	SelectChannel(Channel_SegLED_Digit);
	P0 = 0x01 << pos;
	//发射段选信号，使数码管亮起
	SelectChannel(Channel_SegLED_Segment);
	P0 = segCode;
	//进行少量延时，数码管只能同时亮起一个，延时过多会出现明显的数码管依次亮起
	//不延时如果最后有长延时则会被下次函数调用立即熄灭
	//不延时或延时过短，该数码管其他段会亮起
	for(i=0; i<2000; i++);
}

void SEG_8LED_Number(unsigned long num)
{
	unsigned char i, pos = 7;//从右到左
	if(num == 0)
	{
		SEG_1LED(pos, seg_code[0]);
	}
	while(num != 0)
	{
		i = num % 10;
		num /= 10;
		SEG_1LED(pos, seg_code[i]);
		pos--;
	}
}
/*
void SEG_8LED_String(char* str)
{
	unsigned char i, j;
	unsigned char seg_code[8];
	for(i=0; i<8 && str[i]!=0; i++)
	{
		seg_code[i] = SegCodeCA(str[i]);//SegCodeCA算法复杂具有延时，提前计算
	}
	for(; i<8; i++)
	{
		seg_code[i] = SegCodeCA(' ');
	}
	//while(1)
	{
		for(i=0; i<8 && str[i]!=0; i++)
		{
			SEG_1LED(i, seg_code[i]);
		}
	}
}*/
