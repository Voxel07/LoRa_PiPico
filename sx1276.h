#ifndef SX1276_H
#define SX1276_H

#define REG_LR_VERSION 0x42

#define WRITE_OPERATION 0x7F
#define READ_OPERATION 0xFF

#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <stdio.h>

typedef struct sx1276
{
    spi_inst_t *spi;
    //SPI pins
    uint8_t mosi;
    uint8_t miso;
    uint8_t sck;
    uint8_t cs;
    //Chip Pins
    uint8_t reset;
    uint8_t dio0;
    uint8_t dio1;

} sx1276_t;
void sx1276_init_spi(sx1276_t *sx1276, spi_inst_t *spi, uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t cs, uint8_t reset, uint8_t dio0, uint8_t dio1);

uint8_t SX1276_READ(sx1276_t *sx1276, uint8_t addr);
void SX1276_WRITE(sx1276_t *sy1276, uint8_t addr, uint8_t value);

#endif
