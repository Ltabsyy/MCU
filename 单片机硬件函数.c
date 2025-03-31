#include <reg51.h>

//unsigned char code seg_code[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};//共阴
unsigned char code seg_code[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};//共阳

void select_channel(unsigned char channel)
{
	//通过P2高3位定位通道
	P2 = (P2 & 0x1f) | (0x20 * channel);
	/*switch(channel)
	{
	case 0:
		P2 = (P2 & 0x1f) | 0x00;
		break;
		
	case 1:
		P2 = (P2 & 0x1f) | 0x20;
		break;
		
	case 2:
		P2 = (P2 & 0x1f) | 0x40;
		break;
		
	case 3:
		P2 = (P2 & 0x1f) | 0x60;
		break;
		
	case 4:
		P2 = (P2 & 0x1f) | 0x80;
		break;//LED
		
	case 5:
		P2 = (P2 & 0x1f) | 0xa0;
		break;//蜂鸣器和继电器
		
	case 6:
		P2 = (P2 & 0x1f) | 0xc0;
		break;//数码管位选
		
	case 7:
		P2 = (P2 & 0x1f) | 0xe0;
		break;//数码管段选
	}*/
}

void Init_system()
{
	P2 = 0x00;//P2低5位清0，否则不能选择通道
	select_channel(5);
	P0 = 0x00;//关蜂鸣器和继电器
	select_channel(4);
	P0 = 0xff;//LED全灭
}

void LED8(unsigned char num)
{
	//从低到高的8位，对应从左到右LED，1熄灭0亮起
	select_channel(4);
	P0 = num;
}

void SEG_1LED(unsigned char pos, unsigned char value)
{
	unsigned int i;
	//设置段选全灭，否则其他位会出现残影
	select_channel(7);
	P0 = 0xff;//共阳全灭
	//设置位选信号，定位到单个数码管，数码管从左到右为0-7
	select_channel(6);
	P0 = 0x01 << pos;
	//发射段选信号，使数码管亮起
	select_channel(7);
	P0 = seg_code[value];
	//进行少量延时，数码管只能同时亮起一个，延时过多会出现明显的数码管依次亮起
	//不延时如果最后有长延时则会被下次函数调用立即熄灭
	//不延时或延时过短，该数码管其他段会亮起
	for(i=0; i<2000; i++);
}

void SEG_8LED(unsigned long num)
{
	unsigned char i, pos = 7;//从右到左
	while(num != 0)
	{
		i = num % 10;
		num /= 10;
		SEG_1LED(pos, i);
		pos--;
	}
}

int main()
{
	
	return 0;
}
