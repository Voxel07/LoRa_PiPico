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
#include "LoraMessage.h"
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
    LoraMessage_t LoraMessage;
    // init Lora Module
    printf("Init Lora\n");
    if (lora_begin(&lora, &sx1276, spi0, Frequency_EU868))
    {
        printf("Well shit\n");
        return 1;
    }

    loraMessage_setId(&LoraMessage);

    printf("%llu\n", LoraMessage.messageId);
    printf("%d\n", LoraMessage_getSize(&LoraMessage));

#ifdef RXC
    lora_rx_continuous(&lora);
#endif

#ifdef RXS
    lora_rx_single(&lora);
#endif

#ifdef TX

    LoraMessage_serialize(&LoraMessage);

    int cnt = '0';
    uint8_t arraypls[8] = {'H', 'a', 'l', 'l', 'o', ' ', ' '};

    while (true)
    {
        arraypls[6] = cnt++;
        printf("msg = %s\n", arraypls);
        lora_sendMessage(&lora, arraypls, strlen(arraypls));
        sleep_ms(1000);
        if (cnt >= '9')
            cnt = '0';
    }

#endif

    return 0;
}
