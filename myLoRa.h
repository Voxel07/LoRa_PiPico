#ifndef LORA_H
#define LORA_H

#include "sx1276.h"

#define SF 9                      //7-12
#define Bandwith 125              //125k | 250k
#define Frequency_EU443 443000000 //
#define Frequency_EU868 868000000 //863-870

//Madatory LoRaWAN Channels
#define Madatory_Join_Channel_868_1 868100000
#define Madatory_Join_Channel_868_3 868100000
#define Madatory_Join_Channel_868_5 868100000

//Default TTN Channels
#define Default_TTN_Channel_uplink_867_1 867100000
#define Default_TTN_Channel_uplink_867_3 867300000
#define Default_TTN_Channel_uplink_867_5 867500000
#define Default_TTN_Channel_uplink_867_7 867700000
#define Default_TTN_Channel_uplink_867_9 867900000

#define Default_TTN_Channel_downlink_869_525 869525000 //SF12 FQ125

//Duty Cycles
#define DutyCycle 1 //1% is the maximum allowed by ETSI forr the EU863-870 frequency band
#define SecondsPerDay 86400
#define maxAirTime SecondsPerDay *(DutyCycle / 100)

//Datarates
/**
 * To get the LoRa certification the device must support DR0-DR5
*/
//                     SF  |   BW
#define DR_0 250   //  12  |   125
#define DR_1 440   //  11  |   125
#define DR_2 980   //  10  |   125
#define DR_3 1760  //   9  |   125
#define DR_4 3125  //   8  |   125
#define DR_5 5470  //   7  |   125
#define DR_6 11000 //   7  |   250

//Maximum Payload Size in bytes
#define DR_0_Max_Payload 51
#define DR_1_Max_Payload 51
#define DR_2_Max_Payload 51
#define DR_3_Max_Payload 115
#define DR_4_Max_Payload 242
#define DR_5_Max_Payload 242
#define DR_6_Max_Payload 242

//Delays in s
#define RX1_Delay 1
#define RX2_Delay RX1_Delay + 1
#define Join_Accept_1 5
#define Join_Accept_2 6

//Lora Pins
#define LORA_MOSI 19
#define LORA_MISO 16
#define LORA_SCK 18
#define LORA_CS 8
#define LORA_RESET 9
#define LORA_DIO0 7
#define LORA_DIO1 10

//TX Power values 2 to 20
#define LORA_TX_2 2
#define LORA_TX_17 17
#define LORA_TX_20 20

//REGISTERS

//Freqeuncy
#define REG_FRF_MSB 0x06
#define REG_FRF_MID 0x07
#define REG_FRF_LSB 0x08

//TX
#define REG_PA_CONFIG 0x09
#define REG_PA_DAC 0x4d

#define REG_FIFO_TX_BASE_ADDR 0x0e
#define REG_FIFO_RX_BASE_ADDR 0x0f

#define REG_LNA 0x0c

//setOCP
#define REG_OCP 0x0b

#define REG_MODEM_CONFIG_1 0x1d
#define REG_MODEM_CONFIG_2 0x1e
#define REG_MODEM_CONFIG_3 0x26

#define PA_OUTPUT_RFO_PIN 0
#define PA_OUTPUT_PA_BOOST_PIN 1

// PA config
#define PA_BOOST 0x80
typedef struct lora
{
    sx1276_t *sx1276;
    long _frequency;

} lora_t;

uint8_t lora_begin(lora_t *lora, sx1276_t *sx1276, spi_inst_t *spi, uint8_t address);
void setTxPower(sx1276_t *sx1276, int level, int outputPin);
void lora_setFrequency(lora_t *lora, long frequency);
void setOCP(sx1276_t *sx1276, uint8_t mA);
#endif