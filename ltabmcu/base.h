#ifndef _BASE_H
#define _BASE_H

enum Channel
{
	Channel_LED = 4,//LED
	Channel_Buzz = 5,//蜂鸣器和继电器
	Channel_SegLED_Digit = 6,//数码管位选
	Channel_SegLED_Segment = 7//数码管段选
};
void SelectChannel(unsigned char channel);
void Delay(unsigned int t);//较短的延时，最大65536
void Buzz(char open);//蜂鸣器和继电器响声
void LED8(unsigned char num);
void LED8_ON(unsigned char num);
void InitSystem();

#endif
