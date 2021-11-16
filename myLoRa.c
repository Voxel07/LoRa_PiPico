#include "myLoRa.h"
#include "sx1276.h"

uint8_t lora_begin(lora_t *lora, sx1276_t *sx1276, spi_inst_t *spi, uint8_t addr)
{
    sx1276_init_spi(sx1276, spi, LORA_MOSI, LORA_MISO, LORA_SCK, LORA_CS, LORA_RESET, LORA_DIO0, LORA_DIO1);

    uint8_t version = SX1276_READ(sx1276, addr);

    if (version != 0x12)
    {
        printf("Wrong Version detected: %d", version);
        return 1;
    }
    printf("Setting Lora Frequency \n");
    lora_setFrequency(lora, 868E6);
    // set base addresses
    SX1276_WRITE(sx1276, REG_FIFO_TX_BASE_ADDR, 0);
    SX1276_WRITE(sx1276, REG_FIFO_RX_BASE_ADDR, 0);

    // set LNA boost
    SX1276_WRITE(sx1276, REG_LNA, SX1276_READ(sx1276, REG_LNA) | 0x03);

    // set auto AGC
    SX1276_WRITE(sx1276, REG_MODEM_CONFIG_3, 0x04);

    // set Tx Power
    setTxPower(sx1276, LORA_TX_17, PA_OUTPUT_PA_BOOST_PIN);
    printf("lora begin finished \n");
}

void setTxPower(sx1276_t *sx1276, int level, int outputPin)
{
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

        SX1276_WRITE(sx1276, REG_PA_CONFIG, 0x70 | level);
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
            SX1276_WRITE(sx1276, REG_PA_DAC, 0x87);
            setOCP(sx1276, 140);
        }
        else
        {
            if (level < 2)
            {
                level = 2;
            }
            //Default value PA_HF/LF or +17dBm
            SX1276_WRITE(sx1276, REG_PA_DAC, 0x84);
            setOCP(sx1276, 100);
        }

        SX1276_WRITE(sx1276, REG_PA_CONFIG, PA_BOOST | (level - 2));
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
    lora->_frequency = frequency;

    uint64_t frf = ((uint64_t)frequency << 19) / 32000000;

    SX1276_WRITE(lora->sx1276, REG_FRF_MSB, (uint8_t)(frf >> 16));
    SX1276_WRITE(lora->sx1276, REG_FRF_MID, (uint8_t)(frf >> 8));
    SX1276_WRITE(lora->sx1276, REG_FRF_LSB, (uint8_t)(frf >> 0));
}
/**
 * @brief Set max. current for Over Current Protection control
 *
 * @param sx1276
 * @param mA
 */

void setOCP(sx1276_t *sx1276, uint8_t mA)
{
    uint8_t ocpTrim = 27;

    if (mA <= 120)
    {
        ocpTrim = (mA - 45) / 5;
    }
    else if (mA <= 240)
    {
        ocpTrim = (mA + 30) / 10;
    }

    SX1276_WRITE(sx1276, REG_OCP, 0x20 | (0x1F & ocpTrim));
}
