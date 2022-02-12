#ifndef LORA_H
#define LORA_H

#include "sx1276.h"
#include <ctype.h>

#define SF 9                      // 7-12
#define Bandwith 125              // 125k | 250k
#define Frequency_EU443 443000000 //
#define Frequency_EU868 868000000 // 863-870

// Madatory LoRaWAN Channels
#define Madatory_Join_Channel_868_1 868100000
#define Madatory_Join_Channel_868_3 868100000
#define Madatory_Join_Channel_868_5 868100000

// Default TTN Channels
#define Default_TTN_Channel_uplink_867_1 867100000
#define Default_TTN_Channel_uplink_867_3 867300000
#define Default_TTN_Channel_uplink_867_5 867500000
#define Default_TTN_Channel_uplink_867_7 867700000
#define Default_TTN_Channel_uplink_867_9 867900000

#define Default_TTN_Channel_downlink_869_525 869525000 // SF12 FQ125

// Duty Cycles
#define DutyCycle 1 // 1% is the maximum allowed by ETSI forr the EU863-870 frequency band
#define SecondsPerDay 86400
#define maxAirTime SecondsPerDay *(DutyCycle / 100)

// Datarates
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

// Maximum Payload Size in bytes
#define DR_0_Max_Payload 51
#define DR_1_Max_Payload 51
#define DR_2_Max_Payload 51
#define DR_3_Max_Payload 115
#define DR_4_Max_Payload 242
#define DR_5_Max_Payload 242
#define DR_6_Max_Payload 242

// Delays in s
#define RX1_Delay 1
#define RX2_Delay RX1_Delay + 1
#define Join_Accept_1 5
#define Join_Accept_2 6

// Lora Pins
#define LORA_MOSI 19
#define LORA_MISO 16
#define LORA_SCK 18
#define LORA_CS 8 // change to 17
#define LORA_RESET 9
#define LORA_DIO0 7  // RxDone, TxDone, CadDone Interrupts
#define LORA_DIO1 10 // RxTimeout

// TX Power values 2 to 20
#define LORA_TX_PWR_2 2
#define LORA_TX_PWR_17 17
#define LORA_TX_PWR_20 20

// REGISTERS

// Freqeuncy
#define REG_FRF_MSB 0x06
#define REG_FRF_MID 0x07
#define REG_FRF_LSB 0x08
#define FXOSC 32E6 // fixed value needed for freqeuncy calcualtion

// TX
#define REG_PA_CONFIG 0x09
#define REG_PA_DAC 0x4d
#define MAX_PKT_LENGTH 255
#define MAX_PAYLOAD_LENGTH 128
#define REG_FIFO_TX_BASE_ADDR 0x0e
// #define REG_FIFI_TX_CURRENT_ADDR

// RX
#define REG_FIFO_RX_BASE_ADDR 0x0f
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_LNA 0x0c
#define REG_RX_NB_BYT 0x13
#define REG_PKT_RSSI_VALUE 0x1A
#define REG_RX_HEADER_CNT_VALUE_LSB 0x14
#define REG_RX_HEADER_CNT_VALUE_MSB 0x15
#define REG_RX_HEADER_CNT_VALUE_LSB_2 0x16
#define REG_RX_HEADER_CNT_VALUE_MSB_2 0x17

// setOCP
#define REG_OCP 0x0b

#define REG_MODEM_STAT 0x18
#define REG_MODEM_CONFIG_1 0x1d
#define REG_MODEM_CONFIG_2 0x1e
#define REG_MODEM_CONFIG_3 0x26

// isTransmitting
#define REG_OP_MODE 0x01

// IRQ masks
#define REG_IRQ_FLAGS_MASK 0x11
#define REG_IRQ_FLAGS 0x12
#define IRQ_TX_DONE_MASK 0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
#define IRQ_RX_DONE_MASK 0x40

#define RF_MID_BAND_THRESHOLD 525E6
#define RSSI_OFFSET_HF_PORT 157
#define RSSI_OFFSET_LF_PORT 164

#define PA_OUTPUT_RFO_PIN 0
#define PA_OUTPUT_PA_BOOST_PIN 1
#define REG_DIO_MAPPING_1 0x40
// PA config
#define PA_BOOST 0x80

// modes
#define MODE_TX 0x03
#define MODE_LONG_RANGE_MODE 0x80
#define MODE_SLEEP 0x00
#define MODE_STDBY 0x01
#define MODE_RX_CONTINUOUS 0x05
#define MODE_RX_SINGLE 0x06

// IRQ masks
#define IRQ_TX_DONE_MASK 0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
#define IRQ_RX_DONE_MASK 0x40

// Lora info
#define REG_PAYLOAD_LENGTH 0x22
#define REG_FIFO_ADDR_PTR 0x0d

// FIFO
#define REG_FIFO 0x00 // RegFifoRxBaseAddr

typedef struct loraconfig
{
    bool explicedHeadermode;
    bool enableCRCGeneration;
    uint8_t spreadingfactor;
    uint8_t bandwith;

} loraconfig_t;

typedef struct lora
{
    sx1276_t *sx1276;
    loraconfig_t config;
    long _frequency;
    bool _onTxDone;
    bool _isTransmitting; // usefull?
    int _implicitHeaderMode;

} lora_t;

uint8_t lora_begin(lora_t *lora, sx1276_t *sx1276, spi_inst_t *spi, uint8_t address);
void setTxPower(lora_t *lora, int level, int outputPin);
void lora_setFrequency(lora_t *lora, long frequency);
long lora_getFrequency(lora_t *lora);
void setOCP(lora_t *lora, uint8_t mA);

// Message sending function group
int lora_beginPacket(lora_t *lora, int implicitHeader);
int lora_endPacket(lora_t *lora, bool async);
bool lora_isTransmitting(lora_t *lora);
size_t lora_sendMessage(lora_t *lora, const char *msg, size_t size);

void explicitHeaderMode();
void implicitHeaderMode();

// Message reciving function group
int lora_packetRssi(lora_t *lora);
void lora_printRecivedMessage(lora_t *lora);

// Lora Modes
void lora_goToIdel(lora_t *lora);
void lora_goToSleep(lora_t *lora);
void lora_tx_single(lora_t *lora);
void lora_rx_single(lora_t *lora);
void lora_rx_continuous(lora_t *lora);

// Debug
void lora_Debug(lora_t *lora);

#endif
