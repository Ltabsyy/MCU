#include <reg51.h>//引用P0
#include "base.h"

void LED1(unsigned char pos, unsigned char on)
{
	//仅亮起或熄灭1位
	SelectChannel(Channel_LED);
	if(on)//1亮起0熄灭
	{
		P0 = ~(1 << pos);
	}
	else
	{
		P0 = 1 << pos;
	}
}

void LEDTwinkle()
{
	static unsigned int t = 0;
	t++;//自动溢出归0
	if(t & 0x8) LED8(0xff);
	else LED8(0x00);
}

void LED_Memory(unsigned char pos, unsigned char length, unsigned char off)
{
	static unsigned char lastNum = 0;//初始LED全亮
	unsigned char num = lastNum;
	for(; length > 0; length--)
	{
		//从低到高的8位，对应从左到右LED，1熄灭0亮起
		if(off)
		{
			num |= 1 << pos;
		}
		else
		{
			num &= ~(1 << pos);
		}
		pos++;
	}
	SelectChannel(Channel_LED);
	P0 = num;
	lastNum = num;
}
