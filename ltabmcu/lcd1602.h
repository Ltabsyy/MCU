#ifndef _LCD1602_H
#define _LCD1602_H

void LCD1602_Clear();//清屏
void LCD1602_Return();//回车
void LCD1602_ShowCursor(char visible);//显示或隐藏光标
void LCD1602_Print(char r, unsigned char* str);//显示字符串

#endif
