#ifndef _LORA_MESSAGE_H_
#define _LORA_MESSAGE_H_

// #include <stdio.h>
#include "pico/unique_id.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DS3231.h"

#define NUMBER_OF_SENSORES 4


/**
 * @brief This struct represents the Payload of a Lora Packet
 * @param Id
 * @param timestamp
 * @param sensors
 * @param messageNumer
 * @param messageCount
 */
typedef struct LoraMessage
{
    // uint8_t messsageType;
    uint64_t Id;                                //8bytes
    uint32_t timestamp;                         //4bytes
    uint8_t messageNumer;                       //1bytes
    uint8_t messageCount;                       //1bytes
    uint8_t numSensors;                         //1bytes
    uint32_t pressureData[NUMBER_OF_SENSORES];  //4byte * x
} LoraMessage_t; //31bytes

void loraMessage_init(LoraMessage_t *message);
void LoraMessage_serialize(const LoraMessage_t *message, uint8_t *buffer);
LoraMessage_t LoraMessage_deSerialize(uint8_t msg[]);

#endif