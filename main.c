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

    for (size_t i = 0; i < 100; i++)
    {
        lora_sendMessage(&lora, i, sizeof(i));
        sleep_ms(100);
    }

    return 0;
}
