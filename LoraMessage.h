#ifndef _LORA_MESSAGE_H_
#define _LORA_MESSAGE_H_

#include <stdio.h>
typedef struct LoraMessage
{
    uint32_t timestamp;
    char *msg;

} LoraMessage_t;

#endif