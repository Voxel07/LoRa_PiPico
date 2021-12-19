/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief This is the TX Example
 * @version 0.1
 * @date 2021-12-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "myLoRa.h"

int main()
{
    //init Pi Pico
    stdio_init_all();

    lora_t lora;
    sx1276_t sx1276;

    //init Lora Module
    printf("Init Lora\n");
    if (lora_begin(&lora, &sx1276, spi0, REG_LR_VERSION))
    {
        printf("Well shit\n");
    }

    printf("Value = %d\n", SX1276_READ(&sx1276, 0x01 & READ_OPERATION));

    lora_goToIdel(&sx1276);
    printf("Value = %d\n", SX1276_READ(&sx1276, 0x01 & READ_OPERATION));

    lora_goToSleep(&sx1276);
    printf("Value = %d\n", SX1276_READ(&sx1276, 0x01 & READ_OPERATION));

    uint8_t data[3];
    data[0] = 0x01 & WRITE_OPERATION;
    data[1] = 0x00;
    data[2] = 0x80;
    printf("Value = %d\n", SX1276_READ(&sx1276, 0x01 & READ_OPERATION));

    printf("Size of data= %d\n", sizeof(data) / sizeof(uint8_t));
    gpio_put(sx1276.cs, 0);
    spi_write_blocking(sx1276.spi, data, 3);
    gpio_put(sx1276.cs, 1);
    // SX1276_WRITE(&sx1276, 0x01, 0);
    printf("Value = %d\n", SX1276_READ(&sx1276, 0x01 & READ_OPERATION));
    // SX1276_WRITE(&sx1276, 0x01, 128);
    // printf("Value = %d\n", SX1276_READ(&sx1276, 0x01));
    // SX1276_WRITE(&sx1276, 0x01, 1);
    // printf("Value = %d\n", SX1276_READ(&sx1276, 0x01));

    const char *msg = "HALLO";

    lora_sendMessage(&lora, msg, sizeof(msg));

    return 0;
}
