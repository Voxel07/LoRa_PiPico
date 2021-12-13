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
    printf("Init Lora");
    if (lora_begin(&lora, &sx1276, spi0, REG_LR_VERSION))
    {
        printf("Well shit\n");
    }

    return 0;
}
