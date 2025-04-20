#ifndef _DS1302_H
#define _DS1302_H

struct Time
{
	int year, month, date;
	int hour, minute, second;
	int day;
};
struct Time DS1302_ReadTime();
void DS1302_WriteTime(struct Time time);

#endif
