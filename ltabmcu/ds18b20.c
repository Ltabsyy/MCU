#include "onewire.h"

short ReadTemperatureBin()//读温度字节数据
{
	unsigned char LSB, MSB;
	unsigned short t;
	//启动温度转换
	Init_OneWire();//暂时不考虑复位失败
	Write_OneWire(0xcc);//跳过ROM
	Write_OneWire(0x44);
	Delay_OneWire(5);
	//读取
	Init_OneWire();
	Write_OneWire(0xcc);
	Write_OneWire(0xbe);
	LSB = Read_OneWire();
	MSB = Read_OneWire();
	t = MSB*256+LSB;
	t &= 0x87ff;
	return t;
}

float BinToTemperature(short t)//温度字节数据转实际温度
{
	//符号位0为原码正数，1为补码负数，每1代表1/16摄氏度，利用有符号short型自动处理
	return (float)t/16;
}

float DS18B20_ReadTemperature()//直接返回实际温度
{
	return BinToTemperature(ReadTemperatureBin());
}
