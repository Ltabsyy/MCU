#include "iic.h"

#define PageLength 8//24c02为8，04/08/16为16，32/64为32

void Delay_EEPROM()//写后延时
{
	unsigned int i = 1000;//实测这个值为908时大概率不够，909是够的，取1000保证稳定
	while(i--);
}

void Read_EEPROM(unsigned int address, unsigned char* dataBuf, unsigned char length)
{
	RW_IIC(0xa1, address, dataBuf, length);
}

void Write_EEPROM(unsigned int address, unsigned char* dataBuf, unsigned char length)
{
	unsigned char l;
	if(address/PageLength == (address+length-1)/PageLength)
	{
		RW_IIC(0xa0, address, dataBuf, length);
	}
	else//跨页写入
	{
		//首页
		l = address/PageLength*PageLength+PageLength-1-address+1;
		RW_IIC(0xa0, address, dataBuf, l);
		Delay_EEPROM();
		//后续页可使用递归以简化代码，实际使用非递归以节省栈帧空间
		//Write_EEPROM(address+l, dataBuf+l, length-l);
		//中页
		address += l;
		dataBuf += l;
		length -= l;
		while(length > PageLength)
		{
			RW_IIC(0xa0, address, dataBuf, PageLength);
			Delay_EEPROM();
			address += PageLength;
			dataBuf += PageLength;
			length -= PageLength;
		}
		//末页
		RW_IIC(0xa0, address, dataBuf, length);
		//最后延时由用户控制
	}
}

void WriteDelay_EEPROM(unsigned int address, unsigned char* dataBuf, unsigned char length)//写入并延时，用于连续写
{
	Write_EEPROM(address, dataBuf, length);
	Delay_EEPROM();
}
