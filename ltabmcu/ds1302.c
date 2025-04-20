#include <reg51.h>
#include <intrins.h>//使用_nop_()
#include "ds1302.h"//引用Time结构体

sbit SCK = P1 ^ 7;
sbit SDA = P2 ^ 3;
sbit RST = P1 ^ 3;

#define Address_Second 0x80
#define Address_Minute 0x82
#define Address_Hour 0x84
#define Address_Date 0x86
#define Address_Month 0x88
#define Address_Day 0x8a//星期1-7
#define Address_Year 0x8c
#define Address_WP 0x8e//写入保护控制位

void DS1302_Command(unsigned char c)
{
	unsigned char i;
	for(i=0; i<8; i++)
	{
		SCK = 0;
		SDA = c & 0x01;//发送最低位
		c >>= 1;
		SCK = 1;
	}
}

void DS1302_WriteByte(unsigned char address, unsigned char d)
{
	RST = 0;
	_nop_();
	SCK = 0;
	_nop_();
	RST = 1;
	_nop_();
	DS1302_Command(address & 0xfe);//确保最低位为0
	DS1302_Command(d);
	RST = 0;
}

unsigned char DS1302_ReadByte(unsigned char address)
{
	unsigned char i, temp = 0;
	RST = 0;
	_nop_();
	SCK = 0;
	_nop_();
	RST = 1;
	_nop_();
	DS1302_Command(address | 0x01);//确保最低位为1
	for(i=0; i<8; i++)
	{
		SCK = 0;
		if(SDA)
		{
			temp |= (1 << i);
		}
		SCK = 1;
	}
	RST = 0;
	_nop_();
	SCK = 0;
	_nop_();
	SCK = 1;
	_nop_();
	SDA = 0;
	_nop_();
	SDA = 1;
	_nop_();
	return temp;
}

int ToInt(unsigned char n)
{
	return (n & 0x0f) + 10 * (n >> 4);
}

struct Time DS1302_ReadTime()
{
	struct Time time;
	time.year = ToInt(DS1302_ReadByte(Address_Year));
	time.month = ToInt(DS1302_ReadByte(Address_Month));
	time.date = ToInt(DS1302_ReadByte(Address_Date));
	time.hour = ToInt(DS1302_ReadByte(Address_Hour));
	time.minute = ToInt(DS1302_ReadByte(Address_Minute));
	time.second = ToInt(DS1302_ReadByte(Address_Second));
	time.day = ToInt(DS1302_ReadByte(Address_Day));
	return time;
}

unsigned char ToBit(int n)
{
	return n % 10 | n / 10 << 4;
}

void DS1302_WriteTime(struct Time time)
{
	DS1302_WriteByte(Address_WP, 0x00);//关闭写入保护
	DS1302_WriteByte(Address_Year, ToBit(time.year));
	DS1302_WriteByte(Address_Month, ToBit(time.month));
	DS1302_WriteByte(Address_Date, ToBit(time.date));
	DS1302_WriteByte(Address_Hour, ToBit(time.hour));
	DS1302_WriteByte(Address_Minute, ToBit(time.minute));
	DS1302_WriteByte(Address_Second, ToBit(time.second));
	DS1302_WriteByte(Address_Day, ToBit(time.day));
	DS1302_WriteByte(Address_WP, 0x80);//开启写入保护
}
