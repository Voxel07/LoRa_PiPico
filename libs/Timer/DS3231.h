#ifndef DS3231_H
#define DS3231_H

#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "DateTime.h"

#define TIME_REG 0x00
#define CONTROL_REG 0x0E
#define STATUS_REG 0x0F

#ifndef I2C_SPEED
#define I2C_SPEED
typedef enum
{
    I2C_SLOW = 100000,
    I2C_FAST = 400000
} i2c_speed;
#endif

typedef struct DS3231
{
    i2c_inst_t *i2c;
    uint8_t addr;
    i2c_speed speed;
    uint8_t sda;
    uint8_t scl;
} ds3231_t;

void ds3231_init(ds3231_t *_rtc, i2c_inst_t *_i2c, uint8_t _addr, i2c_speed _speed, uint8_t _sda, uint8_t _scl);
int ds3231_read_bytes(ds3231_t *_rtc, uint8_t _reg, uint8_t *_buf, int _len);
int ds3231_write_bytes(ds3231_t *_rtc, uint8_t _reg, uint8_t *_buf, uint _len);
void ds3231_now(ds3231_t *_rtc, dateTime_t *_dt);
void ds3231_set_datetime(ds3231_t *_rtc, dateTime_t _dt);
void ds3231_set_32KHz_pin(ds3231_t *_rtc, bool _on);
bool ds3231_lost_time(ds3231_t *_rtc);

#endif