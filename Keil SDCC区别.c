/**
 * 以下内容展示Keil和SDCC的代码区别，
 * 每段第一行为Keil，第二行为SDCC
 */

#include <reg52.h>
#include <8052.h>

sfr P0 = 0x80;
__sfr __at (0x80) P0;

sbit P0_1 = P0 ^ 1;
__sbit __at (0x81) P0_1;

unsigned char code a;
__code unsigned char a;

interrupt
__interrupt

#include <intrins.h>
//无
