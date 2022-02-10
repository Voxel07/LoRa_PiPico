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
#include "pico.h"

#define TX 1

int main()
{
    // LED
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    // init Pi Pico
    stdio_init_all();

    lora_t lora;
    sx1276_t sx1276;

    // init Lora Module
    printf("Init Lora\n");
    if (lora_begin(&lora, &sx1276, spi0, REG_LR_VERSION))
    {
        printf("Well shit\n");
        return 1;
    }

#ifdef RXC
    lora_rx_continuous(&lora);
#endif

#ifdef RXS
    lora_rx_single(&lora);
#endif

#ifdef TX
    char *msg = "hallo 0";
    lora_sendMessage(&lora, msg, strlen(msg));
    sleep_ms(1000);
    char *msg2 = "hallo 1";
    lora_sendMessage(&lora, msg2, strlen(msg2));
    sleep_ms(1000);
    char *msg3 = "hallo 2";
    lora_sendMessage(&lora, msg3, strlen(msg3));
    sleep_ms(1000);
    char *msg4 = "hallo 3";
    lora_sendMessage(&lora, msg4, strlen(msg4));
    sleep_ms(1000);

#endif

    return 0;
}
