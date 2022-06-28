#include "DS3231.h"

void ds3231_init(ds3231_t *_rtc, i2c_inst_t *_i2c, uint8_t _addr, i2c_speed _speed, uint8_t _sda, uint8_t _scl)
{
    _rtc->i2c = _i2c;
    _rtc->addr = _addr;
    _rtc->speed = _speed;
    _rtc->sda = _sda;
    _rtc->scl = _scl;
    i2c_init(i2c_default, _rtc->speed);
    gpio_set_function(_rtc->sda, GPIO_FUNC_I2C);
    gpio_set_function(_rtc->scl, GPIO_FUNC_I2C);
    gpio_pull_up(_rtc->sda);
    gpio_pull_up(_rtc->scl);
}

int ds3231_read_bytes(ds3231_t *_rtc, uint8_t _reg, uint8_t *_buf, int _len)
{
    i2c_set_baudrate(_rtc->i2c, _rtc->speed);
    i2c_write_blocking(_rtc->i2c, _rtc->addr, &_reg, 1, true);
    i2c_read_blocking(_rtc->i2c, _rtc->addr, _buf, _len, false);
    return _len;
}

int ds3231_write_bytes(ds3231_t *_rtc, uint8_t _reg, uint8_t *_buf, uint _len)
{
    i2c_set_baudrate(_rtc->i2c, _rtc->speed);
    uint8_t buffer[_len + 1];
    buffer[0] = _reg;
    for (int i = 0; i < _len; i++)
    {
        buffer[i + 1] = _buf[i];
    }
    return i2c_write_blocking(_rtc->i2c, _rtc->addr, buffer, _len + 1, false);
}

void ds3231_now(ds3231_t *_rtc, dateTime_t *_dt)
{

    uint8_t buffer[7];
    int8_t ret = ds3231_read_bytes(_rtc, TIME_REG, buffer, 7);

    uint8_t century = buffer[5] >> 7;
    uint16_t year = (buffer[6] >> 4) * 10 + (buffer[6] & 0x0F) + 2000;
    uint8_t month = ((buffer[5] >> 4) & 0x01) * 10 + (buffer[5] & 0x0F);
    uint8_t day = (buffer[4] >> 4) * 10 + (buffer[4] & 0x0F);
    ;
    uint8_t hour = ((buffer[2] >> 4) & 0x03) * 10 + (buffer[2] & 0x0F);
    uint8_t minute = (buffer[1] >> 4) * 10 + (buffer[1] & 0x0F);
    uint8_t second = (buffer[0] >> 4) * 10 + (buffer[0] & 0x0F);
    uint8_t dow = buffer[3];

    datetime_set(_dt, year, month, day, hour, minute, second);
}

void ds3231_set_datetime(ds3231_t *_rtc, dateTime_t _dt)
{
    uint8_t buffer[7];
    buffer[0] = ((_dt.second / 10) << 4) + (_dt.second % 10);
    buffer[1] = ((_dt.minute / 10) << 4) + (_dt.minute % 10);
    buffer[2] = ((_dt.hour / 10) << 4) + (_dt.hour % 10);
    buffer[3] = ((1 / 10) << 4) + (1 % 10);
    buffer[4] = ((_dt.day / 10) << 4) + (_dt.day % 10);
    buffer[5] = ((_dt.month / 10) << 4) + (_dt.month % 10);
    buffer[6] = (((_dt.year - 2000) / 10) << 4) + ((_dt.year - 2000) % 10);

    ds3231_write_bytes(_rtc, 0, buffer, 7);

    uint8_t buffer2;
    ds3231_read_bytes(_rtc, STATUS_REG, &buffer2, 1);
    buffer2 &= 0b01111111;
    ds3231_write_bytes(_rtc, STATUS_REG, &buffer2, 1); // Reset powerlost bit
}

void ds3231_set_32KHz_pin(ds3231_t *_rtc, bool _on)
{
    uint8_t buffer;
    ds3231_read_bytes(_rtc, STATUS_REG, &buffer, 1);
    if (_on)
    {
        buffer |= 0b00001000;
    }
    else
    {
        buffer &= 0b11110111; // setEN32kHz bit to zero leave rest unchanged
    }

    ds3231_write_bytes(_rtc, STATUS_REG, &buffer, 1);
}

bool ds3231_lost_time(ds3231_t *_rtc)
{
    uint8_t buffer;
    ds3231_read_bytes(_rtc, STATUS_REG, &buffer, 1);

    return buffer >> 7 && 0x01;
}