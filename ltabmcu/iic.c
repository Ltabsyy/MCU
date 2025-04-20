#include <reg51.h>
#include <intrins.h>//使用_nop_()

sbit SCL = P2 ^ 0;
sbit SDA = P2 ^ 1;

void ClockDelay()
{
	unsigned char i = 5;//最开始75，实际0都够用
	while(i--)
	{
		_nop_();
	}
}

void IIC_Start()
{
	SCL = 0;
	ClockDelay();
	SDA = 1;
	ClockDelay();
	SCL = 1;
	ClockDelay();
	SDA = 0;//负跳变
	ClockDelay();
	SCL = 0;
	ClockDelay();
}

void IIC_Stop()
{
	SCL = 0;
	ClockDelay();
	SDA = 0;
	ClockDelay();
	SCL = 1;
	ClockDelay();
	SDA = 1;//正跳变
	ClockDelay();
	SCL = 0;
	ClockDelay();
}

void IIC_ACK()//发送ACK
{
	SCL = 0;
	ClockDelay();
	SDA = 0;
	ClockDelay();
	SCL = 1;
	ClockDelay();
	ClockDelay();
	SCL = 0;
	SDA = 1;//没有这个会使连续读只读到高字节，原因未知
}

void IIC_NoACK()//发送NACK
{
	SCL = 0;
	ClockDelay();
	SDA = 1;
	ClockDelay();
	SCL = 1;
	ClockDelay();
	ClockDelay();
	SCL = 0;
	//SDA = 1;
}

unsigned char IIC_WaitACK()//接收ACK
{
	unsigned char errTime = 255;
	SCL = 0;//确保SCL为0防止SDA更改时产生起止信号
	SDA = 1;//SDA置为1等待拉低
	SCL = 1;
	while(SDA && errTime) errTime--;
	ClockDelay();
	ClockDelay();
	SCL = 0;
	return !!errTime;
}

void IIC_SendByte(unsigned char b)
{
	unsigned char i;
	SCL = 0;
	for(i=0; i<8; i++)//从高到低
	{
		ClockDelay();
		SDA = !!(b & 0x80);//发送最高位
		//ClockDelay();
		SCL = 1;
		ClockDelay();
		b <<= 1;
		SCL = 0;
	}
}

unsigned char IIC_ReceiveByte()
{
	char i;
	unsigned char b = 0;
	SCL = 0;
	for(i=7; i>=0; i--)//从高到低
	{
		ClockDelay();
		SCL = 1;
		ClockDelay();
		if(SDA == 1)
		{
			b |= (1 << i);//存入对应位
		}
		SCL = 0;
		//ClockDelay();
	}
	return b;
}

void RW_ICC(unsigned char select, unsigned int address, unsigned char* dataBuf, unsigned char length)
{
	char i;
	IIC_Start();
	IIC_SendByte(select & 0xfe);
	if(!IIC_WaitACK()) return;
	//IIC_SendByte((unsigned char)(address >> 8));//器件若为24c32及以上，地址是双字节，先发高字节，不过我们是24c02
	//if(!IIC_WaitACK()) return;
	IIC_SendByte((unsigned char)address);
	if(!IIC_WaitACK()) return;
	if(select & 0x01)//读
	{
		IIC_Start();
		IIC_SendByte(select);
		if(!IIC_WaitACK()) return;
		for(i=0; i<length-1; i++)
		{
			dataBuf[i] = IIC_ReceiveByte();
			IIC_ACK();
			ClockDelay();
		}
		dataBuf[length-1] = IIC_ReceiveByte();
		IIC_NoACK();
	}
	else//写
	{
		for(i=0; i<length; i++)
		{
			IIC_SendByte(dataBuf[i]);
			if(!IIC_WaitACK()) return;
		}
	}
	IIC_Stop();
}
