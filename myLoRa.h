#ifndef LORA_H
#define LORA_H

#include "sx1276.h"
#include <ctype.h>
#include "LoraDefines.h"
#include <stdlib.h>
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
    bool _implicitHeaderMode;

} lora_t;

// Setup functions
uint8_t lora_begin(lora_t *lora, sx1276_t *sx1276, spi_inst_t *spi, uint32_t frequency);
void lora_setTxPower(lora_t *lora, int level, int outputPin);
void lora_setBandwith(lora_t *lora, uint8_t BW);
void lora_setCodingRate(lora_t *lora, uint8_t CR);
void lora_SpreadingFactor(lora_t *lora, uint8_t SF);
void lora_setFrequency(lora_t *lora, long frequency);
long lora_getFrequency(lora_t *lora);
void lora_setOCP(lora_t *lora, uint8_t mA);

// Message sending function group
int lora_beginPacket(lora_t *lora, int implicitHeader);
int lora_endPacket(lora_t *lora, bool async);
bool lora_isTransmitting(lora_t *lora);
size_t lora_sendMessage(lora_t *lora, const uint8_t *msg, size_t size);

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
