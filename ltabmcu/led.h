#ifndef _LED_H
#define _LED_H

//提供较为高级的LED控制，基本控制在base中
void LED1(unsigned char pos, unsigned char on);
void LEDTwinkle();
void LED_Memory(unsigned char pos, unsigned char length, unsigned char off);

#endif
