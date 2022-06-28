#ifndef LORA_DEFINES_H
#define LORA_DEFINES_H

// Lora Spreading Factor
#define LoRa_SF_6 6 //64 chips / symbo
#define LoRa_SF_7 7 //128 chips / symbol
#define LoRa_SF_8 8 //256 chips / symbol
#define LoRa_SF_9 9 //512 chips / symbol
#define LoRa_SF_10 10 //1024 chips / symbol
#define LoRa_SF_11 11 //2048 chips / symbol
#define LoRa_SF_12 12 //4096 chips / symbol

// Lora Bandwith
#define LoRa_BW_0 0x00 //7.8kHz
#define LoRa_BW_1 0x01 //10.4kHz
#define LoRa_BW_2 0x02 //15.6kHz
#define LoRa_BW_3 0x03 //20.8kHz
#define LoRa_BW_4 0x04 //31.25kHz
#define LoRa_BW_5 0x05 //41.7kHz
#define LoRa_BW_6 0x06 //62.5kHz
#define LoRa_BW_7 0x07 //1235kHz
#define LoRa_BW_8 0x08 //250kHz
#define LoRa_BW_9 0x09 //500kHz

// Lora Cyclic Coding Overhead
#define LoRa_CR_1 1 //4/5
#define LoRa_CR_2 2 //4/6
#define LoRa_CR_3 3 //4/7
#define LoRa_CR_4 4 //4/8

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
#define DR_4_Max_Payload 222
#define DR_5_Max_Payload 222
#define DR_6_Max_Payload 222

// Delays in s
#define RX1_Delay 1
#define RX2_Delay RX1_Delay + 1
#define Join_Accept_1 5
#define Join_Accept_2 6

// Lora Pins | gpio nr // pin nr
#define LORA_MOSI 19 // 25
#define LORA_MISO 16 // 21
#define LORA_SCK 18
#define LORA_CS 8 // change to 17

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
#endif