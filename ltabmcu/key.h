#ifndef _KEY_H
#define _KEY_H

void Delay_Key();
unsigned char ButtonPressed_S(unsigned char n);//检测S7-S4按键是否被按住，J5需为BTN
unsigned char ButtonDown_S(unsigned char n);//检测S7-S4按键，按下触发，J5需为BTN
unsigned char ButtonUp_S(unsigned char n);//检测S7-S4按键，弹起触发，J5需为BTN
unsigned char KeyBoardPressedAloneNumber();//键盘仅单键被按住时，返回按键序号，J5需为KBD
unsigned char KeyBoardDownAloneNumber();//键盘仅单键按下时，返回按键序号，J5需为KBD

#endif
