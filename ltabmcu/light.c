#include "iic.h"

void RD1_ReadLight(unsigned char* pLight)//读取光照强度
{
	RW_ICC(0x91, 0x01, pLight, 1);
}

float LightToVolte(unsigned char light)//光照强度转电压
{
	return (float)light/51;//255相当于5V
}
