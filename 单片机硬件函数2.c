#include <reg51.h>

void Delay(unsigned int t)//较短的延时，最大65536
{
	while(t--);
}

// 基本元件控制
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

void InitSystem()
{
	// 使系统在LCD插上时正常运行
	//P2 = 0x00;//P2低5位清0，否则不能选择通道
	P1 &= 0xfb;//P1.2的E设为0屏蔽LCD信号
	
	SelectChannel(Channel_Buzz);
	P0 = 0x00;//关蜂鸣器和继电器
	SelectChannel(Channel_LED);
	P0 = 0xff;//LED全灭
}

void Buzz(unsigned char open)//蜂鸣器和继电器响声
{
	static unsigned char last = 0;
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

void LEDTwinkle()
{
	static unsigned int t = 0;
	t++;//自动溢出归0
	if(t & 0x8) LED8(0xff);
	else LED8(0x00);
}

// 数码管字形代码
struct Seg
{
	char ch;
	char a, b, c, d, e, f, g, dp;
};

struct Seg code segList[] = {
	{' ', 0, 0, 0, 0, 0, 0, 0, 0},
	{'-', 0, 0, 0, 0, 0, 0, 1, 0},
	{'.', 0, 0, 0, 0, 0, 0, 0, 1},
	{'0', 1, 1, 1, 1, 1, 1, 0, 0},
	{'1', 0, 1, 1, 0, 0, 0, 0, 0},
	{'2', 1, 1, 0, 1, 1, 0, 1, 0},
	{'3', 1, 1, 1, 1, 0, 0, 1, 0},
	{'4', 0, 1, 1, 0, 0, 1, 1, 0},
	{'5', 1, 0, 1, 1, 0, 1, 1, 0},
	{'6', 1, 0, 1, 1, 1, 1, 1, 0},
	{'7', 1, 1, 1, 0, 0, 0, 0, 0},
	{'8', 1, 1, 1, 1, 1, 1, 1, 0},
	{'9', 1, 1, 1, 1, 0, 1, 1, 0},
	{'A', 1, 1, 1, 0, 1, 1, 1, 0},
	{'a', 1, 1, 1, 1, 1, 0, 1, 0},
	{'b', 0, 0, 1, 1, 1, 1, 1, 0},
	{'C', 1, 0, 0, 1, 1, 1, 0, 0},
	{'c', 0, 0, 0, 1, 1, 0, 1, 0},
	{'d', 0, 1, 1, 1, 1, 0, 1, 0},
	{'E', 1, 0, 0, 1, 1, 1, 1, 0},
	{'F', 1, 0, 0, 0, 1, 1, 1, 0},
	{'G', 1, 0, 1, 1, 1, 1, 0, 0},
	{'H', 0, 1, 1, 0, 1, 1, 1, 0},
	{'J', 0, 1, 1, 1, 0, 0, 0, 0},
	{'L', 0, 0, 0, 1, 1, 1, 0, 0},
	{'n', 0, 0, 1, 0, 1, 0, 1, 0},
	{'o', 0, 0, 1, 1, 1, 0, 1, 0},
	{'P', 1, 1, 0, 0, 1, 1, 1, 0},
	{'q', 1, 1, 1, 0, 0, 1, 1, 0},
	{'r', 0, 0, 0, 0, 1, 0, 1, 0},
	{'U', 0, 1, 1, 1, 1, 1, 0, 0},
	{'u', 0, 0, 1, 1, 1, 0, 0, 0},
	{'y', 0, 1, 1, 1, 0, 1, 1, 0},
	{'_', 0, 0, 0, 1, 0, 0, 0, 0}
};

struct Seg SearchSeg(char c)
{
	char i, n = sizeof(segList)/sizeof(struct Seg);
	for(i = 0; i < n; i++)
	{
		if(segList[i].ch == c)
		{
			return segList[i];
		}
	}
	return SearchSeg(' ');
}

unsigned char SegCodeCC(char c)//共阴
{
	struct Seg seg;
	unsigned char segCode;
	seg = SearchSeg(c);
	segCode = seg.dp << 7;
	segCode += seg.g << 6;
	segCode += seg.f << 5;
	segCode += seg.e << 4;
	segCode += seg.d << 3;
	segCode += seg.c << 2;
	segCode += seg.b << 1;
	segCode += seg.a << 0;
	return segCode;
}

unsigned char SegCodeCA(char c)//共阳
{
	return 0xff - SegCodeCC(c);
}

// 数码管显示
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
	//unsigned char code seg_code[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};//共阴
	unsigned char code seg_code[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};//共阳
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
}

// 按键
int S(unsigned char n)//检测S7-S4按键，弹起触发，J5需为BTN
{
	static unsigned char last[8] = {0};
	if(!(P3 & (1 << (7 - n))))
	{
		last[n] = 1;
	}
	else if(last[n])
	{
		last[n] = 0;
		return 1;
	}
	return 0;
}

sbit KR0 = P3 ^ 0;
sbit KR1 = P3 ^ 1;
sbit KR2 = P3 ^ 2;
sbit KR3 = P3 ^ 3;
sbit KC0 = P4 ^ 4;
sbit KC1 = P4 ^ 2;
sbit KC2 = P3 ^ 5;
sbit KC3 = P3 ^ 4;
unsigned char GetKeyCode()//检测当前按下键代码，J5需为KBD
{
	unsigned char c = 0;
	//行检测
	KR0 = 1, KR1 = 1, KR2 = 1, KR3 = 1;
	KC0 = 0, KC1 = 0, KC2 = 0, KC3 = 0;
	c += (KR0 == 1) << 0;
	c += (KR1 == 1) << 1;
	c += (KR2 == 1) << 2;
	c += (KR3 == 1) << 3;
	if(c == 0x0f) return 0xff;//没有任何键按下
	//列检测
	KR0 = 0, KR1 = 0, KR2 = 0, KR3 = 0;
	KC0 = 1, KC1 = 1, KC2 = 1, KC3 = 1;
	c += (KC0 == 1) << 4;
	c += (KC1 == 1) << 5;
	c += (KC2 == 1) << 6;
	c += (KC3 == 1) << 7;
	return c;
}

int main()
{
	
	return 0;
}
