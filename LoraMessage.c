#include "LoraMessage.h"

void loraMessage_init(LoraMessage_t *message)
{
    uint64_t Id;
    pico_unique_board_id_t picoId;
    pico_get_unique_board_id(&picoId);
    memcpy(&Id, picoId.id, sizeof(Id));
    message->Id = Id;

    ds3231_t rtc;
    ds3231_init(&rtc,i2c0,0x68,I2C_FAST,12,13);
    ds3231_set_32KHz_pin(&rtc, false);
    dateTime_t dt_now;
    ds3231_now(&rtc, &dt_now);
    message->timestamp = datetime_to_epoch(&dt_now);

    for (size_t i = 0; i < NUMBER_OF_SENSORES; i++)
    {
        message->pressureData[i] = rand();  // call sensor func
    }

    message->messageCount = 1; // Enought space to send all data at once
    message->messageNumer = 1;
    message->numSensors = NUMBER_OF_SENSORES;
}

void LoraMessage_serialize(const LoraMessage_t *message, uint8_t *buffer)
{
    uint8_t msg[31] = {0};

    memcpy(&msg[0], &message->Id, sizeof(message->Id)); // Id           - Byte 0-7
    memcpy(&msg[8], &message->timestamp, sizeof(message->timestamp)); // Timestamp    - Byte 8-11
    msg[12] = message->messageNumer;                                  // MessageNr    - Byte 12
    msg[13] = message->messageCount;                                  // MessageCount - Byte 13
    msg[14] = message->numSensors;                                    // SensorCount  - Byte 14
    for (uint8_t sensor = 0; sensor < message->numSensors; sensor++)
    {
        memcpy(&msg[15 + sizeof(uint32_t)*sensor], &message->pressureData[sensor], sizeof(uint32_t)); // SensorData
    }

    memcpy(buffer,msg,31);
}

LoraMessage_t LoraMessage_deSerialize(uint8_t msg[])
{
    LoraMessage_t message;

    memcpy(&message.Id, &msg[0], sizeof(message.Id)); // Id          - Byte 0-7
    memcpy(&message.timestamp, &msg[8], sizeof(message.timestamp)); // Timestamp   - Byte 8-11
    message.messageNumer = msg[12];                                // MessageNr    - Byte 12
    message.messageCount = msg[13];                                // MessageCount - Byte 13
    message.numSensors = msg[14];                                  // SensorCount  - Byte 14
    for (uint8_t sensor = 0; sensor < message.numSensors; sensor++)
    {
        memcpy(&message.pressureData[sensor], &msg[15 + sizeof(uint32_t)*sensor], sizeof(uint32_t)); // SensorData
    }

    return message;
}