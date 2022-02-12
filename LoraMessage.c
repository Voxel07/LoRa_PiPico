#include "LoraMessage.h"

/**
 * @brief This adds a unique Id to the Lora Message. This is used to identify the Message on the reciving end.
 * The Id is the unique Id of the PiPico the programm is run on.
 *
 * @param message
 */
void loraMessage_setId(LoraMessage_t *message)
{
    uint64_t Id;
    pico_unique_board_id_t picoId;

    pico_get_unique_board_id(&picoId);
    memcpy(&Id, picoId.id, sizeof(Id));
    message->messageId = Id;
}

/**
 * @brief Set the Timestamp to the Lora Message. This is used to validate Packets on the reciving end.
 * The Time is taken form an external DS3231 RTC module
 *
 * @param message
 */
void loraMessage_setTimeStamp(LoraMessage_t *message)
{
    message->timestamp = 123456789;
}

/**
 * @brief Set the pressure Sensor data to the Lora Message
 *
 * @param message
 * @param sensors
 * @param seonsorCount
 */
void loraMessage_setSensoreData(LoraMessage_t *message, PressureSensor_t sensors[NUMBER_OF_SENSORES], uint8_t seonsorCount)
{
    for (size_t i = 0; i < seonsorCount; i++)
    {
        message->sensors[i].SensoreId = sensors[i].SensoreId;
        message->sensors[i].pressure = sensors[i].pressure;
    }
}

/**
 * @brief Set the Number of the current packet beeing send. This is used when the payload is to big for a single message.
 * The reciver can then rebuild the complete message with this information.
 *
 * @param message
 * @param number
 */
void loraMessage_setMessageNumber(LoraMessage_t *message, uint8_t number)
{
    message->messageNumer = 1;
}

/**
 * @brief Increaces the Number of the current packet. Has to be called after every succesfull tx.
 *
 * @param message
 */
void loraMessage_incMessageNumber(LoraMessage_t *message)
{
    message->messageNumer++;
}

/**
 * @brief Set the total number of packets when the payload is to big
 *
 * @param message
 * @param count
 */
void loraMessage_setMessageCount(LoraMessage_t *message, uint8_t count)
{
    message->messageCount = 10;
}

/**
 * @brief Returns the current size of the LoraMessage in Byte
 *
 * @param message
 * @return uint8_t
 */
uint8_t LoraMessage_getSize(LoraMessage_t *message)
{
    return sizeof(message->messageId) + sizeof(message->timestamp) + sizeof(message->messageNumer) + sizeof(message->messageCount) + sizeof(message->sensors->pressure) * NUMBER_OF_SENSORES + sizeof(message->sensors->SensoreId) * NUMBER_OF_SENSORES;
}

uint8_t *LoraMessage_serialize(LoraMessage_t *message)
{
    uint8_t *msg = malloc(50);

    for (int i = 0; i < 8; ++i)
    {
        msg[i] = (unsigned char)((((unsigned long long)message->messageId) >> (56 - (8 * i))) & 0xFFu);
    }
    return msg;
}

void LoraMessage_deSerialize(LoraMessage_t *message, uint8_t *msg, uint8_t length)
{
}