#ifndef _ONEWIRE_H
#define _ONEWIRE_H

void Delay_OneWire(unsigned int t);
void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20();
bit Init_DS18B20();

#endif
