#ifndef _LORA_MESSAGE_H_
#define _LORA_MESSAGE_H_

// #include <stdio.h>
#include "pressure.h"
#include "pico/unique_id.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_SENSORES 4

/**
 * @brief This struct represents the Payload of a Lora Packet
 * @param messageId
 * @param timestamp
 * @param sensors
 * @param messageNumer
 * @param messageCount
 */
typedef struct LoraMessage
{
    uint64_t messageId;
    uint32_t timestamp;
    PressureSensor_t sensors[NUMBER_OF_SENSORES];
    uint8_t messageNumer;
    uint8_t messageCount;
} LoraMessage_t;

void loraMessage_setId(LoraMessage_t *message);
void loraMessage_setTimeStamp(LoraMessage_t *message);
void loraMessage_setSensoreData(LoraMessage_t *message, PressureSensor_t sensors[NUMBER_OF_SENSORES], uint8_t seonsorCount);
void loraMessage_setMessageNumber(LoraMessage_t *message, uint8_t number);
void loraMessage_incMessageNumber(LoraMessage_t *message);
void loraMessage_setMessageCount(LoraMessage_t *message, uint8_t count);
uint8_t LoraMessage_getSize(LoraMessage_t *message);
uint8_t *LoraMessage_serialize(LoraMessage_t *message);
void LoraMessage_deSerialize(LoraMessage_t *message, uint8_t *msg, uint8_t length);

#endif