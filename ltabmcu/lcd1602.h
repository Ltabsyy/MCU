#ifndef _LCD1602_H
#define _LCD1602_H

void LCD1602_Clear();//清屏
void LCD1602_Return();//回车
void LCD1602_ShowCursor(char visible);//显示或隐藏光标
void LCD1602_Init();//初始化，必须首先调用
void LCD1602_Print(char r, char c, unsigned char* str);//显示字符串
void LCD1602_Printf(char r, char c, unsigned char* format, ...);//LCD上格式化输出，常量整数参数必须显式(int)转换

#endif
