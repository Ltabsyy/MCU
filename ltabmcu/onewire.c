#include <reg51.h>

sbit DQ = P1 ^ 4;

void Delay_OneWire(unsigned int t)
{
	t *= 10;
	while(t--);
}

void Write_OneWire(unsigned char d)//写，变量不能命名为data关键字
{
	unsigned char i;
	for(i=0; i<8; i++)//从低到高写位
	{
		DQ = 0;
		DQ = d & 0x01;//写最低位
		Delay_OneWire(5);
		DQ = 1;
		d >>= 1;
	}
	Delay_OneWire(5);
}

unsigned char Read_OneWire()//读
{
	unsigned char i, d = 0;
	for(i=0; i<8; i++)//从低到高读位
	{
		DQ = 0;
		d >>= 1;
		DQ = 1;
		if(DQ)
		{
			d |= 0x80;//存入最高位
		}
		Delay_OneWire(5);
	}
	return d;
}

bit Init_OneWire()//复位和应答
{
	bit initFlag = 0;
	DQ = 1;
	Delay_OneWire(12);
	DQ = 0;
	Delay_OneWire(80);
	DQ = 1;
	Delay_OneWire(10);
	initFlag = DQ;
	Delay_OneWire(5);
	return initFlag;
}
