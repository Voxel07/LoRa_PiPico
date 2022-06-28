#include <stdio.h>
#include "pico/stdlib.h"
#include "myLoRa.h"
#include "pico.h"
#include "LoraMessage.h"

int main()
{
    lora_t lora;
    sx1276_t sx1276;
    LoraMessage_t LoraMessage;

    if (lora_begin(&lora, &sx1276, spi0, Frequency_EU443))
    {
        printf("Init failed \n");
        return 1;
    }

    loraMessage_init(&LoraMessage);
    uint8_t buffer[50];
    LoraMessage_serialize(&LoraMessage,buffer);
    LoraMessage.messageCount = 100;
    for (size_t i = 0; i <= LoraMessage.messageCount; i++)
    {
        LoraMessage.messageNumer = i;
        LoraMessage_serialize(&LoraMessage,buffer);
        lora_sendMessage(&lora,buffer,sizeof(buffer));
        if(LoraMessage.messageNumer == LoraMessage.messageCount){
            i = 0;
        }
    }
    return 0;
}
