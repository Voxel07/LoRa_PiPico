#ifndef SX1276_H
#define SX1276_H

#define REG_LR_VERSION 0x42

#define WRITE_OPERATION 0x80
#define READ_OPERATION 0x7F

// Register
#define RegBitrateMsb 0x02
#define RegBitrateLsb 0x03

#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <string.h>
#include <stdio.h>

typedef struct sx1276
{
    spi_inst_t *spi;

    uint8_t _mosi;
    uint8_t _miso;
    uint8_t _sck;
    uint8_t _cs;

} sx1276_t;

void sx1276_init_spi(sx1276_t *sx1276, spi_inst_t *spi, uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t cs);

uint8_t SX1276_READ_SINGLE_BYTE(sx1276_t *sx1276, uint8_t addr);
void SX1276_READ(sx1276_t *sx1276, uint8_t addr, size_t len, uint8_t *data);
void SX1276_WRITE_SINGLE_BYTE(sx1276_t *sx1276, uint8_t addr, uint8_t data);
void SX1276_WRITE(sx1276_t *sx1276, uint8_t addr, const char *data, size_t len);

#endif
