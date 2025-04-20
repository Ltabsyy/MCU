#include <reg51.h>

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

void Delay(unsigned int t)
{
	while(t--);
}

void LCD1602_WriteCommand(unsigned char c)
{
	E = 0;
	RS = 0;//命令
	Delay(50);
	RW = 0;//写
	Delay(50);
	DB = c;
	Delay(50);
	E = 1;
	Delay(50);
	E = 0;
}

void LCD1602_WriteData(unsigned char d)
{
	E = 0;
	RS = 1;//数据
	Delay(50);
	RW = 0;//写
	Delay(50);
	DB = d;
	Delay(50);
	E = 1;
	Delay(50);
	E = 0;
}

void LCD1602_Clear()//清屏
{
	LCD1602_WriteCommand(Command_ClearScreen);
	Delay(500);
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
	Delay(500);
}

void LCD1602_Init()//初始化
{
	LCD1602_WriteCommand(Command_SetWorkMode | WorkMode_DL | WorkMode_N);//设置为8位2行
	Delay(500);
	LCD1602_ShowCursor(0);//显示开，无光标
	LCD1602_WriteCommand(Command_SetMode | Mode_ID);//光标右移，显示不移动
	Delay(500);
	LCD1602_Clear();
}

void LCD1602_Print(char r, unsigned char* str)//显示字符串
{
	static unsigned char isInited = 0;
	unsigned char i, address = 0;
	if(!isInited) LCD1602_Init();
	if(r) address = DDAddress_SecondLine;
	LCD1602_WriteCommand(Command_SetDDRAMAddress | address);
	for(i=0; str[i] && i < DDAddress_LineEnd; i++)
	{
		LCD1602_WriteData(str[i]);
	}
}

