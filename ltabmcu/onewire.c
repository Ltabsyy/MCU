#include <reg51.h>

sbit DQ = P1^4;

void Delay_OneWire(unsigned int t)
{
	while(t--);
}

void Write_DS18B20(unsigned char dat)//写，变量不能命名为data关键字
{
	unsigned char i;
	for(i=0; i<8; i++)//从低到高写位
	{
		DQ = 0;
		DQ = dat & 0x01;//写最低位
		Delay_OneWire(50);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(50);
}

unsigned char Read_DS18B20()//读
{
	unsigned char i, dat = 0;
	for(i=0; i<8; i++)//从低到高读位
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;//存入最高位
		}
		Delay_OneWire(50);
	}
	return dat;
}

bit Init_DS18B20()//复位和应答
{
	bit initflag = 0;
	DQ = 1;
	Delay_OneWire(120);
	DQ = 0;
	Delay_OneWire(800);
	DQ = 1;
	Delay_OneWire(100);
	initflag = DQ;
	Delay_OneWire(50);
	return initflag;
}
