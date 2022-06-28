#include "DateTime.h"

void datetime_set(dateTime_t *_dt, uint16_t _year, uint8_t _month, uint8_t _day, uint8_t _hour, uint8_t _minute, uint8_t _second)
{
    _dt->year = _year;
    _dt->month = _month;
    _dt->day = _day;
    _dt->hour = _hour;
    _dt->minute = _minute;
    _dt->second = _second;
}

void datetime_to_string(dateTime_t *_dt, char *_str)
{
    sprintf(_str, "%d.%02d.%02d %02d:%02d:%02d\0", _dt->year, _dt->month, _dt->day, _dt->hour, _dt->minute, _dt->second);
}

uint32_t datetime_to_epoch(dateTime_t *_dt)
{
    uint32_t t_now = (_dt->year - 1970) * 31556926L + (_dt->month - 1) * 2629743L + (_dt->day - 1) * 86400L + _dt->hour * 3600 + _dt->minute * 60 + _dt->second;
    return t_now;
}

void datetime_from_string(dateTime_t *dt, char *_str)
{
    dt->year = atoi(strtok(_str, "/")) + 2000;
    dt->month = atoi(strtok(NULL, "/"));
    dt->day = atoi(strtok(NULL, ","));
    dt->hour = atoi(strtok(NULL, ":"));
    dt->minute = atoi(strtok(NULL, ":"));
    dt->second = atoi(strtok(NULL, "+-"));
    // dt->dts = atoi(strtok(NULL, "\"\0\n"));
}