#ifndef _EEPROM_H
#define _EEPROM_H

void Delay_EEPROM();
void Read_EEPROM(unsigned int address, unsigned char* dataBuf, unsigned char length);
void Write_EEPROM(unsigned int address, unsigned char* dataBuf, unsigned char length);
void WriteDelay_EEPROM(unsigned int address, unsigned char* dataBuf, unsigned char length);//写入并延时，用于连续写

#endif
