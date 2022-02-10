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

    int cnt = '0';
    char arraypls[8] = {'H', 'a', 'l', 'l', 'o', ' ', ' ', '\n'};
    char *zulang = "123456789123456789123465789123465798132465798132465789132456798ablololololololtopkekdas ist doch dumm warum ist der Sepciher so klein Speicher schreibt man nicht so, ah yes sepciher o.O <-Gimme some space-> 123456789123456789123465789123465798132465798132465789132456798ablololololololtopkekdas ist doch dumm warum ist der Sepciher so klein Speicher schreibt man nicht so, ah yes sepciher o.O";
    // 123456789123456789123465789123465798132465798132465789132456798ablololololololtopkekdas ist doch dumm warum ist der Sepciher so klein Speicher schreibt man nicht so, ah yes sepciher o.O
    lora_sendMessage(&lora, zulang, strlen(zulang));

    while (true)
    {
        // It werks ! ayyyyyyyyyyyyyy
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
