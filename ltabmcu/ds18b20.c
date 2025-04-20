#include "onewire.h"

short ReadTemperatureBin()
{
	unsigned char LSB, MSB;
	unsigned short t;
	//启动温度转换
	Init_DS18B20();//暂时不考虑复位失败
	Write_DS18B20(0xcc);//跳过ROM
	Write_DS18B20(0x44);
	Delay_OneWire(50);
	//读取
	Init_DS18B20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);
	LSB = Read_DS18B20();
	MSB = Read_DS18B20();
	t = MSB*256+LSB;
	t &= 0x87ff;
	return t;
}
