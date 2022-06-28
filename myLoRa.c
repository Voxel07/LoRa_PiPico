#include "myLoRa.h"
#include "sx1276.h"

/**
 * @brief
 *
 * @param lora
 * @param sx1276
 * @param spi
 * @param addr
 * @return uint8_t
 */
uint8_t lora_begin(lora_t *lora, sx1276_t *sx1276, spi_inst_t *spi, uint32_t frequency)
{
    lora->sx1276 = sx1276;

    sx1276_init_spi(sx1276, spi, LORA_MOSI, LORA_MISO, LORA_SCK, LORA_CS);

    uint8_t version = SX1276_READ_SINGLE_BYTE(sx1276, REG_LR_VERSION);

    if (version != 0x12)
    {
        printf("Wrong Version detected: %d \n", version);
        return 1;
    }
    // Set the Modem into Lora Mode
    lora_goToSleep(lora);
    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_OP_MODE, 0x80);

    printf("Setting Lora Frequency \n");
    lora_setFrequency(lora, frequency);
    // set base addresses
    SX1276_WRITE_SINGLE_BYTE(sx1276, REG_FIFO_TX_BASE_ADDR, 0x80);
    SX1276_WRITE_SINGLE_BYTE(sx1276, REG_FIFO_RX_BASE_ADDR, 0);
    // set LNA boost
    SX1276_WRITE_SINGLE_BYTE(sx1276, REG_LNA, SX1276_READ_SINGLE_BYTE(sx1276, REG_LNA) | 0x03);
    // set auto Error coding rate
    SX1276_WRITE_SINGLE_BYTE(sx1276, REG_MODEM_CONFIG_3, 0x04);
    // set Tx Power
    lora_setTxPower(lora, LORA_TX_PWR_17, PA_OUTPUT_PA_BOOST_PIN);
    printf("lora begin finished \n");

    return 0;
}

void lora_setBandwith(lora_t *lora, uint8_t BW)
{
    //Default is 0x07 -> 125kHz
    if(BW < LoRa_BW_0 || BW > LoRa_BW_9) return; //Filer invalide Bandwith
    if(lora->_frequency >= 169E6 && BW>=LoRa_BW_8) return; //Filter not supportet cobinations

    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_MODEM_CONFIG_1, SX1276_READ_SINGLE_BYTE(lora->sx1276,REG_MODEM_CONFIG_1)||BW);
}

void lora_setCodingRate(lora_t *lora, uint8_t CR)
{
    //Default is 0x01 4/5
    if (CR < LoRa_CR_1 || CR > LoRa_CR_4 ) return; //Filer invalide CodingRate
    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_MODEM_CONFIG_1, SX1276_READ_SINGLE_BYTE(lora->sx1276,REG_MODEM_CONFIG_1)||CR);
}

/**
 * @brief
 * https://www.thethingsnetwork.org/airtime-calculator
 * @param lora
 * @param SF
 */
void lora_SpreadingFactor(lora_t *lora, uint8_t SF)
{
    //Default is 0x07 -> 64 chips / symbol
    if(SF == LoRa_SF_6)
    {
        lora->_implicitHeaderMode = true;
        SX1276_WRITE_SINGLE_BYTE(lora->sx1276,REG_MODEM_CONFIG_1,SX1276_READ_SINGLE_BYTE(lora->sx1276,REG_MODEM_CONFIG_1)||0x01);
    }
    else if(SF < LoRa_SF_6 && SF >= LoRa_SF_12)
    {
        if (lora->_frequency > 196E6 && (SF == LoRa_SF_8 ||SF == LoRa_SF_9))
        {
            printf("SF8 & SF9 do not work with a frequency of %lu\n",lora->_frequency);
            return;
        }

        SX1276_WRITE_SINGLE_BYTE(lora->sx1276,REG_MODEM_CONFIG_1,SF);
    }
    else
    {
        printf("Invalide SF recived.\n");
    }
}

/**
 * @brief Set the Tx Power object
 *
 * @param sx1276
 * @param level
 * @param outputPin
 */
void lora_setTxPower(lora_t *lora, int level, int outputPin)
{
    printf("setTxPower \n");
    if (PA_OUTPUT_RFO_PIN == outputPin)
    {
        // RFO
        if (level < 0)
        {
            level = 0;
        }
        else if (level > 14)
        {
            level = 14;
        }

        SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_PA_CONFIG, 0x70 | level);
    }
    else
    {
        // PA BOOST
        if (level > 17)
        {
            if (level > 20)
            {
                level = 20;
            }

            // subtract 3 from level, so 18 - 20 maps to 15 - 17
            level -= 3;

            // High Power +20 dBm Operation (Semtech SX1276/77/78/79 5.4.3.)
            SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_PA_DAC, 0x87);
            lora_setOCP(lora, 140);
        }
        else
        {
            if (level < 2)
            {
                level = 2;
            }
            // Default value PA_HF/LF or +17dBm
            SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_PA_DAC, 0x84);
            lora_setOCP(lora, 100);
        }

        SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_PA_CONFIG, PA_BOOST | (level - 2));
    }
}

/**
 * @brief Set the transmittig frequency of the Lora module
 * This needs an Initalized Lora struct and the choosen frequency
 *
 * @param lora
 * @param frequency
 */
void lora_setFrequency(lora_t *lora, long frequency)
{
    printf("lora_setFrequency \n");
    lora->_frequency = frequency;

    uint64_t frf = ((uint64_t)frequency << 19) / 32000000;

    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_FRF_MSB, (uint8_t)(frf >> 16));
    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_FRF_MID, (uint8_t)(frf >> 8));
    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_FRF_LSB, (uint8_t)(frf >> 0));
}

// long lora_getFrequency(lora_t *lora)
// {
//     return (unsigned long)((double)(((unsigned long)SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_FRF_MSB) << 16) |
//                                     ((unsigned long)SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_FRF_MID) << 8) |
//                                     ((unsigned long)SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_FRF_LSB))) *
//                            FREQ_STEP);
// }

/**
 * @brief Set max. current for Over Current Protection control
 *
 * @param sx1276
 * @param mA
 */
void lora_setOCP(lora_t *lora, uint8_t mA)
{
    printf("setOCP \n");

    uint8_t ocpTrim = 27;

    if (mA <= 120)
    {
        ocpTrim = (mA - 45) / 5;
    }
    else if (mA <= 240)
    {
        ocpTrim = (mA + 30) / 10;
    }

    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_OCP, 0x20 | (0x1F & ocpTrim));
}

void explicitHeaderMode(lora_t *lora)
{
    lora->_implicitHeaderMode = 0;

    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_MODEM_CONFIG_1, SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_MODEM_CONFIG_1) & 0xfe);
}

void implicitHeaderMode(lora_t *lora)
{
    lora->_implicitHeaderMode = 1;

    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_MODEM_CONFIG_1, SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_MODEM_CONFIG_1) | 0x01);
}

/**
 * @brief Check if the lora module is currently transmitting data to prevent data loss.
 *
 * @return true
 * @return false
 */
bool lora_isTransmitting(lora_t *lora)
{
    printf("lora_isTransmitting \n");

    // A Message is beeing sent at the moment
    if ((SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_OP_MODE) & MODE_TX) == MODE_TX)
    {
        return true;
    }

    // No message is beeing sent
    if (SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK)
    {
        // clear IRQ's
        SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
    }

    return false;
}

/**
 * @brief This function should be called bevor a new Message can be send. To ensure a clean state.
 *
 * @param implicitHeader
 * @return int
 */
int lora_beginPacket(lora_t *lora, int implicitHeader)
{
    printf("lora_beginPacket \n");

    // Check whether a message is currently being sent
    if (lora_isTransmitting(lora))
    {
        return 0;
    }

    // Set Lora in Idle state to start sending a new message
    lora_goToIdel(lora);

    if (implicitHeader)
    {
        implicitHeaderMode(lora);
    }
    else
    {
        explicitHeaderMode(lora); // default
    }

    // reset FIFO address and paload length
    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_FIFO_ADDR_PTR, 0x80);
    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_PAYLOAD_LENGTH, 0x00);
    return 1;
}

/**
 * @brief This function Sends the Package. It can be choosen if this task is done blocking aka sync or non blocking aka async
 *
 * @param lora
 * @param async
 * @return int
 */
int lora_endPacket(lora_t *lora, bool async)
{

    if ((async) && (lora->_onTxDone))
    {
        SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_DIO_MAPPING_1, 0x40); // DIO0 => TXDONE
    }

    // put in TX mode
    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

    if (!async)
    {
        // wait for TX done
        while ((SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0)
        {
            //
        }
        // clear IRQ's
        SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
    }

    return 1;
}

/**
 * @brief This function sends the pared message.
 * If the message is to long it will be spilt and send in multible bursts
 *
 * @param msg
 * @param size
 * @return size_t
 */
size_t lora_sendMessage(lora_t *lora, const uint8_t *msg, size_t size)
{
    printf("lora_sendMessage\n");
    lora_beginPacket(lora, lora->_implicitHeaderMode); // Explicit Header Mode

    // seralize data bevor sending it

    int currentLength = SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_PAYLOAD_LENGTH);

    printf("Currents fifo size: %d \n", currentLength);
    printf("Currents  size: %d \n", size);
    for (size_t i = 0; i < size; i++)
    {
        printf("%d",msg[i]);
    }
    printf("\n");


    // check if the new msg fits into the fifo register
    if ((currentLength + size) < MAX_PAYLOAD_LENGTH)
    {

        SX1276_WRITE(lora->sx1276, REG_FIFO, msg, size);

        // update length
        SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_PAYLOAD_LENGTH, currentLength + size);

        lora_endPacket(lora, false);
    }
    else // Split msg
    {
        // size = MAX_PKT_LENGTH - currentLength;
        // The Message needs to be send in single packages
    }

    return size;
}

int lora_packetRssi(lora_t *lora)
{
    //RSSI Value depends on which output Port was used
    return (SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_PKT_RSSI_VALUE) - (lora->_frequency < RF_MID_BAND_THRESHOLD ? RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT));
}

// Lora Modes
void lora_goToIdel(lora_t *lora)
{
    printf("lora_goToIdle \n");

    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
}

// Will wipe FIFO Buffer
void lora_goToSleep(lora_t *lora)
{
    printf("lora_goToSleep \n");

    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
}

void lora_tx_single(lora_t *lora)
{
    printf("Lora_tx_single\n");
    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

    while ((SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0)
    {
        // Wait for TX done
    }
}

void lora_rx_single(lora_t *lora)
{
    printf("Lora_rx_single\n");
    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_SINGLE);
    while ((SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_IRQ_FLAGS) & IRQ_RX_DONE_MASK) == 0)
    {
    }

    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    printf("Recived a message\n");
    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_IRQ_FLAGS, IRQ_RX_DONE_MASK);
    // Call ISR
    lora_printRecivedMessage(lora);
    // break;
}

void lora_rx_continuous(lora_t *lora)
{
    printf("Lora_rx_continuous\n");

    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS);

    while (true)
    {
        if (!(SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_IRQ_FLAGS) & IRQ_RX_DONE_MASK) == 0)
        {
            SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_IRQ_FLAGS, IRQ_RX_DONE_MASK); // Reset IRQ
            // Do Stuff
            lora_printRecivedMessage(lora);
        }
    }
}

void lora_getFiFoData(lora_t *lora, uint8_t *data, uint8_t *length)
{
    // Extract Payload from FIFO
    uint8_t currentPos = SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_FIFO_RX_CURRENT_ADDR);
    *length = SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_RX_NB_BYT);
    uint8_t retData[*length];

    /*
     * Muss für den Impliziten typ noch angepasst werden
     */

    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_FIFO_ADDR_PTR, currentPos); // Set FiFo ptr to corrert position
    SX1276_READ(lora->sx1276, REG_FIFO, *length, data);                    // Get FiFo data

}

/**
 * @brief
 *
 * @param lora
 */
void lora_printRecivedMessage(lora_t *lora)
{

    // Extract Payload from FIFO
    uint8_t currentPos = SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_FIFO_RX_CURRENT_ADDR);
    uint8_t length = SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_RX_NB_BYT);
    uint8_t data[length];

    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_FIFO_ADDR_PTR, currentPos); // Set FiFo ptr to corrert position
    SX1276_READ(lora->sx1276, REG_FIFO, length, data);                     // Get FiFo data
    // printf("length: %d\n",length);
    // Print the FiFo data
    for (size_t i = 0; i < length; i++)
    {
        data[i] = toupper(data[i]);
    }

    for (size_t i = 0; i < length; i++)
    {
        printf("%d", data[i]);
        //If chars change to %c
    }
    printf("\n");

    printf("RSSI : %d\n", lora_packetRssi(lora)); // Print paket infos
}

/**
 * @brief This Function will print all relevant Register
 *
 * @param lora
 */
void lora_Debug(lora_t *lora)
{
    printf("----------------------------------------------------------------\n");
    printf("OpMode: %x\n", SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_OP_MODE));
    // printf("Freq: %d\n", lora_getFrequency(lora));

    printf("IRQ FLags Mask: %x\n", SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_IRQ_FLAGS_MASK));
    printf("IRQ FLags: %x\n", SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_IRQ_FLAGS));

    printf("Bytes last package: %x\n", SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_RX_NB_BYT));
    printf("Cnt valid headers: %x|%x\n", SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_RX_HEADER_CNT_VALUE_LSB), (SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_RX_HEADER_CNT_VALUE_MSB) << 8));
    printf("Cnt valid headers2: %x|%x\n", SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_RX_HEADER_CNT_VALUE_LSB_2), (SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_RX_HEADER_CNT_VALUE_MSB_2) << 8));

    printf("Modem stat: %x\n", SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_MODEM_STAT));
    printf("REG_MODEM_CONFIG_1: %x\n", SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_MODEM_CONFIG_1));
    printf("REG_MODEM_CONFIG_2: %x\n", SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_MODEM_CONFIG_2));
    printf("REG_MODEM_CONFIG_3: %x\n", SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_MODEM_CONFIG_3));

    printf("FiFo Ptr Pos: %x\n", SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_FIFO_ADDR_PTR));
    printf("FiFo payload len %x\n", SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_PAYLOAD_LENGTH));
}
