#ifndef _ONEWIRE_H
#define _ONEWIRE_H

void Delay_OneWire(unsigned int t);
void Write_OneWire(unsigned char d);
unsigned char Read_OneWire();
bit Init_OneWire();

#endif
