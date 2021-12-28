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
uint8_t lora_begin(lora_t *lora, sx1276_t *sx1276, spi_inst_t *spi, uint8_t addr)
{
    lora->sx1276 = sx1276;

    sx1276_init_spi(sx1276, spi, LORA_MOSI, LORA_MISO, LORA_SCK, LORA_CS, LORA_RESET, LORA_DIO0, LORA_DIO1);

    uint8_t version = SX1276_READ_SINGLE_BYTE(sx1276, addr);

    if (version != 0x12)
    {
        printf("Wrong Version detected: %d \n", version);
        return 1;
    }

    printf("Setting Lora Frequency \n");
    lora_setFrequency(lora, 868E6);

    // set base addresses
    SX1276_WRITE_SINGLE_BYTE(sx1276, REG_FIFO_TX_BASE_ADDR, 0);
    SX1276_WRITE_SINGLE_BYTE(sx1276, REG_FIFO_RX_BASE_ADDR, 0);

    // set LNA boost
    SX1276_WRITE_SINGLE_BYTE(sx1276, REG_LNA, SX1276_READ_SINGLE_BYTE(sx1276, REG_LNA) | 0x03);

    // set auto AGC
    SX1276_WRITE_SINGLE_BYTE(sx1276, REG_MODEM_CONFIG_3, 0x04);

    // set Tx Power
    setTxPower(sx1276, LORA_TX_17, PA_OUTPUT_PA_BOOST_PIN);
    printf("lora begin finished \n");

    return 0;
}

/**
 * @brief Set the Tx Power object
 *
 * @param sx1276
 * @param level
 * @param outputPin
 */
void setTxPower(sx1276_t *sx1276, int level, int outputPin)
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

        SX1276_WRITE_SINGLE_BYTE(sx1276, REG_PA_CONFIG, 0x70 | level);
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
            SX1276_WRITE_SINGLE_BYTE(sx1276, REG_PA_DAC, 0x87);
            setOCP(sx1276, 140);
        }
        else
        {
            if (level < 2)
            {
                level = 2;
            }
            //Default value PA_HF/LF or +17dBm
            SX1276_WRITE_SINGLE_BYTE(sx1276, REG_PA_DAC, 0x84);
            setOCP(sx1276, 100);
        }

        SX1276_WRITE_SINGLE_BYTE(sx1276, REG_PA_CONFIG, PA_BOOST | (level - 2));
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

/**
 * @brief Set max. current for Over Current Protection control
 *
 * @param sx1276
 * @param mA
 */
void setOCP(sx1276_t *sx1276, uint8_t mA)
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

    SX1276_WRITE_SINGLE_BYTE(sx1276, REG_OCP, 0x20 | (0x1F & ocpTrim));
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
bool lora_isTransmitting(sx1276_t *sx1276)
{
    printf("lora_isTransmitting \n");

    // A Message is beeing sent at the moment
    if ((SX1276_READ_SINGLE_BYTE(sx1276, REG_OP_MODE) & MODE_TX) == MODE_TX)
    {
        return true;
    }

    // No message is beeing sent
    // C
    if (SX1276_READ_SINGLE_BYTE(sx1276, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK)
    {
        // clear IRQ's
        SX1276_WRITE_SINGLE_BYTE(sx1276, REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
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

    //Check whether a message is currently being sent
    if (lora_isTransmitting(lora->sx1276))
    {
        return 0;
    }

    // Set Lora in Idle state to start sending a new message
    lora_goToIdel(lora->sx1276);

    if (implicitHeader)
    {
        implicitHeaderMode(lora);
    }
    else
    {
        explicitHeaderMode(lora);
    }

    // reset FIFO address and paload length
    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_FIFO_ADDR_PTR, 0);
    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_PAYLOAD_LENGTH, 0);

    return 1;
}

int lora_endPacket(lora_t *lora, bool async)
{

    if ((async) && (lora->_onTxDone))
        SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_DIO_MAPPING_1, 0x40); // DIO0 => TXDONE

    // put in TX mode
    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

    if (!async)
    {
        // wait for TX done
        while ((SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0)
        {
            //https://github.com/arduino-libraries/Scheduler/blob/master/src/Scheduler.cpp
            // yield();
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
size_t lora_sendMessage(lora_t *lora, const char *msg, size_t size)
{
    lora_beginPacket(lora, 1);
    //seralize data bevor sending it

    printf("lora_sendMessage\n");

    int currentLength = SX1276_READ_SINGLE_BYTE(lora->sx1276, REG_PAYLOAD_LENGTH);

    printf("Currents fifo size: %d \n", currentLength);

    // check if the new msg fits into the fifo register
    if ((currentLength + size) > MAX_PKT_LENGTH)
    {
        size = MAX_PKT_LENGTH - currentLength;
    }

    // write data
    for (size_t i = 0; i < size; i++)
    {
        printf("Writing data %d\n", i);
        SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_FIFO, msg[i]);
    }

    // update length
    SX1276_WRITE_SINGLE_BYTE(lora->sx1276, REG_PAYLOAD_LENGTH, currentLength + size);

    lora_endPacket(lora, true);
    return size;
}

void lora_goToIdel(sx1276_t *sx1276)
{
    printf("lora_goToIdle \n");

    SX1276_WRITE_SINGLE_BYTE(sx1276, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
}
void lora_goToSleep(sx1276_t *sx1276)
{
    printf("lora_goToSleep \n");

    SX1276_WRITE_SINGLE_BYTE(sx1276, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
}