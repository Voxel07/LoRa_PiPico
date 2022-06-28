#ifndef DATETIME_H
#define DATETIME_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DateTime
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} dateTime_t;

void datetime_set(dateTime_t *_dt, uint16_t _year, uint8_t _month, uint8_t _day, uint8_t _hour, uint8_t _minute, uint8_t _second);
void datetime_to_string(dateTime_t *_dt, char *_str);
uint32_t datetime_to_epoch(dateTime_t *_dt);
void datetime_from_string(dateTime_t *_dt, char *_str);

#endif