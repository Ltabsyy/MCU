#include <reg51.h>//引用P3，避免链接时重复定义
//sfr P3 = 0xb0;
sfr P4 = 0xc0;

sbit KR0 = P3 ^ 0;
sbit KR1 = P3 ^ 1;
sbit KR2 = P3 ^ 2;
sbit KR3 = P3 ^ 3;
sbit KC0 = P4 ^ 4;
sbit KC1 = P4 ^ 2;
sbit KC2 = P3 ^ 5;
sbit KC3 = P3 ^ 4;

unsigned short GetKeyState()
{
	unsigned char rs = 0;
	unsigned short keystate = 0;//16位视为4*4矩阵
	//行快速判断
	KR0 = 1, KR1 = 1, KR2 = 1, KR3 = 1;
	KC0 = 0, KC1 = 0, KC2 = 0, KC3 = 0;
	rs += (KR0 == 0) << 0;//0为按下
	rs += (KR1 == 0) << 1;
	rs += (KR2 == 0) << 2;
	rs += (KR3 == 0) << 3;
	if(!rs) return 0;//无键按下
	if(rs | 1)
	{
		KR0 = 0, KR1 = 1, KR2 = 1, KR3 = 1;
		KC0 = 1, KC1 = 1, KC2 = 1, KC3 = 1;
		keystate += (KC0 == 0) << 0;
		keystate += (KC1 == 0) << 1;
		keystate += (KC2 == 0) << 2;
		keystate += (KC3 == 0) << 3;
	}
	if(rs | 2)
	{
		KR0 = 1, KR1 = 0, KR2 = 1, KR3 = 1;
		KC0 = 1, KC1 = 1, KC2 = 1, KC3 = 1;
		keystate += (KC0 == 0) << 4;
		keystate += (KC1 == 0) << 5;
		keystate += (KC2 == 0) << 6;
		keystate += (KC3 == 0) << 7;
	}
	if(rs | 4)
	{
		KR0 = 1, KR1 = 1, KR2 = 0, KR3 = 1;
		KC0 = 1, KC1 = 1, KC2 = 1, KC3 = 1;
		keystate += (KC0 == 0) << 8;
		keystate += (KC1 == 0) << 9;
		keystate += (KC2 == 0) << 10;
		keystate += (KC3 == 0) << 11;
	}
	if(rs | 8)
	{
		KR0 = 1, KR1 = 1, KR2 = 1, KR3 = 0;
		KC0 = 1, KC1 = 1, KC2 = 1, KC3 = 1;
		keystate += (KC0 == 0) << 12;
		keystate += (KC1 == 0) << 13;
		keystate += (KC2 == 0) << 14;
		keystate += (KC3 == 0) << 15;
	}
	return keystate;
}

unsigned char KeyTrigger(unsigned char slot, unsigned char isPressedNow,
	unsigned char downTrigger, unsigned char upTrigger, unsigned int cycleTrigger)//按键触发器，统一管理按键消抖
{
	static unsigned int cache[20]={0};
	if(isPressedNow)
	{
		if(downTrigger && cache[slot] == 0)
		{
			cache[slot]++;
			return 1;
		}
		cache[slot]++;
		if(cycleTrigger && cache[slot] == cycleTrigger)
		{
			cache[slot] = 1;
			return 1;
		}
		if(cache[slot] == 0xffff) cache[slot] = 1;//int溢出
	}
	else
	{
		if(upTrigger && cache[slot] != 0)
		{
			cache[slot] = 0;
			return 1;
		}
		cache[slot] = 0;
	}
	return 0;
}

void KeyTriggerInit()
{
	unsigned char n;
	for(n=0; n<20; n++)
	{
		KeyTrigger(n, 0, 0, 0, 0);
	}
}

void Delay_Key()
{
	unsigned int t = 10000;
	while(t--);
}

unsigned char ButtonPressed_S(unsigned char n)//检测S7-S4按键是否被按住，J5需为BTN
{
	return !(P3 & (1 << (7 - n)));
}

unsigned char ButtonDown_S(unsigned char n)//检测S7-S4按键，按下触发，J5需为BTN
{
	if(KeyTrigger(n, ButtonPressed_S(n), 1, 0, 0))
	{
		Delay_Key();
		return ButtonPressed_S(n);
	}
	return 0;
}

unsigned char ButtonUp_S(unsigned char n)//检测S7-S4按键，弹起触发，J5需为BTN
{
	if(KeyTrigger(n, ButtonPressed_S(n), 0, 1, 0))
	{
		Delay_Key();
		return !ButtonPressed_S(n);
	}
	return 0;
}
/*
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

void KeyCodeTranslate(unsigned char co, char* r, char* c)
{
	unsigned char ch, cl;
	co = ~co;
	cl = co & 0x0f;
	ch = co >> 4;
	if(cl == 1) *r = 0;
	else if(cl == 2) *r = 1;
	else if(cl == 4) *r = 2;
	else if(cl == 8) *r = 3;
	else *r = -1;
	if(ch == 1) *c = 0;
	else if(ch == 2) *c = 1;
	else if(ch == 4) *c = 2;
	else if(ch == 8) *c = 3;
	else *c = -1;
}
*/
unsigned char KeystateToAloneNumber(unsigned short keystate)
{
	unsigned char r = 0, c = 0;
	if(!keystate) return 0;
	for(; !(keystate & 1 << c); c++);
	r = c >> 2;
	c %= 4;
	return 7-r+4*c;//返回S4-S19
}

unsigned char KeyBoardPressedAloneNumber()//键盘仅单键被按住时，返回按键序号，J5需为KBD
{
	return KeystateToAloneNumber(GetKeyState());
}

unsigned char KeyBoardDownAloneNumber()//键盘仅单键按下时，返回按键序号，J5需为KBD
{
	static unsigned char last = 0;
	unsigned char n, r;
	n = KeyBoardPressedAloneNumber();
	r = KeyTrigger(n?n:last, n, 1, 0, 0)*n;
	last = n;
	if(r)
	{
		Delay_Key();
		return (KeyBoardPressedAloneNumber() == r)*r;
	}
	return 0;
}
/*
unsigned char KeyBoardUpAloneNumber()//键盘仅单键按下时，返回按键序号，J5需为KBD
{
	static unsigned char last = 0;
	unsigned char n, r;
	n = KeyBoardPressedAloneNumber();
	r = KeyTrigger(n?n:last, n, 0, 1, 0)*n;
	last = n;
	if(r)
	{
		Delay_Key();
		return (!GetKeyState())*r;
	}
	return 0;
}
*/
