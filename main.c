#include <stdio.h>
#include "pico/stdlib.h"
#include "myLoRa.h"

int main()
{
    lora_t lora;
    sx1276_t sx1276;

    stdio_init_all();
    printf("Init Lora");
    lora_begin(&lora, &sx1276, spi0, REG_LR_VERSION);
}
