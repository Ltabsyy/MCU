#include <reg51.h>

enum Channel
{
	Channel_LED = 4,//LED
	Channel_Buzz = 5,//蜂鸣器和继电器
	Channel_SegLED_Digit = 6,//数码管位选
	Channel_SegLED_Segment = 7//数码管段选
};

void SelectChannel(unsigned char channel)
{
	//通过P2高3位定位通道
	P2 = (P2 & 0x1f) | (0x20 * channel);
}

void Delay(unsigned int t)//较短的延时，最大65536
{
	while(t--);
}

void Buzz(char open)//蜂鸣器和继电器响声
{
	static char last = -1;
	if(open != last)
	{
		SelectChannel(Channel_Buzz);
		if(open) P0 = 0x40;//0x40仅响蜂鸣，0xd0也响继电
		else P0 = 0x00;
		last = open;
	}
}

void LED8(unsigned char num)
{
	//从低到高的8位，对应从左到右LED，1熄灭0亮起
	SelectChannel(Channel_LED);
	P0 = num;
}

void LED8_ON(unsigned char num)//1亮起0熄灭
{
	SelectChannel(Channel_LED);
	P0 = ~num;
}

void InitSystem()
{
	// 使系统在LCD插上时正常运行
	//P2 = 0x00;//P2低5位清0，否则不能选择通道
	P1 &= 0xfb;//P1.2的E设为0屏蔽LCD信号
	
	Buzz(0);//关蜂鸣器和继电器
	LED8_ON(0);//LED全灭
}
