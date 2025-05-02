#include <reg51.h>
#include <stdarg.h>//使用va_list
#include <stdio.h>//使用vsprintf()

sbit RS = P2 ^ 0;
sbit RW = P2 ^ 1;
sbit E = P1 ^ 2;
#define DB P0

//指令
#define Command_SetDDRAMAddress 0x80//设置显示数据RAM地址
#define DDAddress_SecondLine 0x40//第二行/第一行
#define DDAddress_LineEnd 40//行超尾地址，每行内存40列，显示前16列

#define Command_SetCGRAMAddress 0x40//设置字符产生RAM地址

#define Command_SetWorkMode 0x20
#define WorkMode_DL 0x10//8位数据接口/4位数据接口
#define WorkMode_N 0x08//两行显示/一行显示
#define WorkMode_F 0x04//5*10点阵/5*8点阵，两行显示时总为5*8

#define Command_Move 0x10
#define Move_SC 0x08//显示+光标移动/仅光标移动
#define Move_RL 0x04//右移/左移

#define Command_SetDisplay 0x08
#define Display_D 0x04//显示开/关
#define Display_C 0x02//光标显示/不显示
#define Display_B 0x01//光标闪烁/不闪烁

#define Command_SetMode 0x04
#define Mode_ID 0x02//写入新数据后光标右移/左移
#define Mode_S 0x01//显示移动/不移动

#define Command_CursorReturn 0x02//回车，光标返回左上角，不改变显示内容

#define Command_ClearScreen 0x01//清屏

void Delay_LCD1602()//E高脉冲延时
{
	unsigned int t = 20;//该值建议至少为10，取20保证稳定
	while(t--);
}

void LCD1602_WriteCommand(unsigned char c)
{
	E = 0;
	RS = 0;//命令
	RW = 0;//写
	DB = c;
	Delay_LCD1602();
	E = 1;
	Delay_LCD1602();
	E = 0;
}

void LCD1602_WriteData(unsigned char d)
{
	E = 0;
	RS = 1;//数据
	RW = 0;//写
	DB = d;
	Delay_LCD1602();
	E = 1;
	Delay_LCD1602();
	E = 0;
}
/*
unsigned char LCD1602_ReadState()
{
	unsigned char s;
	E = 0;
	RS = 0;//状态
	RW = 1;//读
	Delay_LCD1602();
	E = 1;
	Delay_LCD1602();
	s = DB;
	E = 0;
	return s;
}

unsigned char LCD1602_ReadData()
{
	unsigned char d;
	E = 0;
	RS = 1;//数据
	RW = 1;//读
	Delay_LCD1602();
	E = 1;
	Delay_LCD1602();
	d = DB;
	E = 0;
	return d;
}

unsigned char LCD1602_IsBusy()
{
	return !!(LCD1602_ReadState() & 0x80);
}

unsigned char LCD1602_ReadAddressCounter()
{
	return LCD1602_ReadState() & 0x7f;
}
*/
void LCD1602_Clear()//清屏，不能在初始化前调用，否则会使第二行不显示
{
	unsigned int t = 1000;//清屏需要独立长延时，否则会出现第一行无法列定位，首个或更多字符不显示等问题
	LCD1602_WriteCommand(Command_ClearScreen);
	while(t--);
}

void LCD1602_Return()//回车
{
	LCD1602_WriteCommand(Command_CursorReturn);
}

void LCD1602_ShowCursor(char visible)//显示或隐藏光标
{
	if(visible)
	{
		LCD1602_WriteCommand(Command_SetDisplay | Display_D | Display_C | Display_B);//光标总是闪烁
	}
	else
	{
		LCD1602_WriteCommand(Command_SetDisplay | Display_D);
	}
}

void LCD1602_Init()//初始化
{
	LCD1602_WriteCommand(Command_SetWorkMode | WorkMode_DL | WorkMode_N);//设置为8位2行
	LCD1602_ShowCursor(0);//显示开，无光标
	LCD1602_WriteCommand(Command_SetMode | Mode_ID);//光标右移，显示不移动
	LCD1602_Clear();
}

void LCD1602_Print(char r, char c, unsigned char* str)//显示字符串
{
	unsigned char i, address = c;
	if(r) address |= DDAddress_SecondLine;
	LCD1602_WriteCommand(Command_SetDDRAMAddress | address);
	for(i=0; str[i] && i < DDAddress_LineEnd; i++)
	{
		LCD1602_WriteData(str[i]);
	}
}

void LCD1602_Printf(char r, char c, unsigned char* format, ...)//LCD上格式化输出
{
	char str[DDAddress_LineEnd] = {0};
	va_list v_list;
	va_start(v_list, format);//接收可变参数列表
	vsprintf(str, format, v_list);
	va_end(v_list);
	LCD1602_Print(r, c, str);
}
/*
void LCD1602_SetCustomChar(unsigned char slot, unsigned char height, unsigned char** dotMatrix)//槽位0-7，点阵为8*5矩阵或槽位0-3，点阵为10*5矩阵
{
	unsigned char table[10] = {0};
	unsigned char r, c;
	if(height > 10) height = 10;
	for(r=0; r<height; r++)
	{
		for(c=0; c<5; c++)
		{
			table[r] += dotMatrix[r][c] << (4-c);
		}
	}
	if(height <= 8) LCD1602_WriteCommand(Command_SetCGRAMAddress | slot * 0x08);
	else LCD1602_WriteCommand(Command_SetCGRAMAddress | slot * 0x10);
	for(r=0; r<height; r++)
	{
		LCD1602_WriteData(table[r]);
	}
}

void LCD1602_ShowCustomChar(char r, char c, unsigned char slot, unsigned char height)
{
	unsigned char address = c;
	if(r) address |= DDAddress_SecondLine;
	LCD1602_WriteCommand(Command_SetDDRAMAddress | address);
	if(height <= 8) LCD1602_WriteData(slot);
	else LCD1602_WriteData(slot*2);
}*/
