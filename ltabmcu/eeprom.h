#ifndef _EEPROM_H
#define _EEPROM_H

void Delay_EEPROM();
void Read_EEPROM(unsigned int address, unsigned char* dataBuf, unsigned char length);
void Write_EEPROM(unsigned int address, unsigned char* dataBuf, unsigned char length);

#endif
