#include "sx1276.h"

/**
 * @brief This function initalices the SPI communication to the SX1276 LoRa chip.
 *
 * @param sx1276
 * @param spi
 * @param mosi
 * @param miso
 * @param sck
 * @param cs
 */
void sx1276_init_spi(sx1276_t *sx1276, spi_inst_t *spi, uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t cs)
{
    printf("init SPI\n");
    // init spi
    sx1276->spi = spi;

    // init pins
    sx1276->_mosi = mosi;
    sx1276->_miso = miso;
    sx1276->_sck = sck;
    sx1276->_cs = cs;

    // validate
    spi_init(spi0, 10 * 1000 * 1000);
    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    // set Lora pins
    gpio_set_function(sx1276->_mosi, GPIO_FUNC_SPI);
    gpio_set_function(sx1276->_miso, GPIO_FUNC_SPI);
    gpio_set_function(sx1276->_sck, GPIO_FUNC_SPI);

    // init gpios
    gpio_init(sx1276->_cs);
    gpio_set_dir(sx1276->_cs, GPIO_OUT);
    gpio_put(sx1276->_cs, 1);
}

/**
 * @brief Read the value of a single 8 Bit register at addr
 *
 * @param sx1276
 * @param addr
 * @return uint8_t
 */
uint8_t SX1276_READ_SINGLE_BYTE(sx1276_t *sx1276, uint8_t addr)
{

    gpio_put(sx1276->_cs, 0); // start communication
    uint8_t rxData = addr & READ_OPERATION; //Set the first bit for read

    // Check if we can write data to the spi device
    if (spi_is_writable(sx1276->spi))
    {
        spi_write_blocking(sx1276->spi, &rxData, 1); // select the register
    }
    else
    {
        printf("not writeable\n");
    }

    spi_read_blocking(sx1276->spi, 0, &rxData, 1); // read value of register

    gpio_put(sx1276->_cs, 1); // stop communication

    return rxData;
}

/**
 * @brief Read len number of bytes from the register at addr.
 *
 * @param sx1276
 * @param addr
 * @param len
 * @param data
 */
void SX1276_READ(sx1276_t *sx1276, uint8_t addr, size_t len, uint8_t *data)
{
    gpio_put(sx1276->_cs, 0); // start communication
    uint8_t dst = addr & READ_OPERATION; //Set the first bit for read

    // Check if we can write data to the spi device
    if (spi_is_writable(sx1276->spi))
    {
        spi_write_blocking(sx1276->spi, &dst, 1); // select the register
    }
    else
    {
        printf("not writeable\n");
    }

    spi_read_blocking(sx1276->spi, 0, data, len); // read value of register

    gpio_put(sx1276->_cs, 1); // stop communication
}

/**
 * @brief Write the value of data to the register at addr
 *
 * @param sx1276
 * @param addr
 * @param data
 */
void SX1276_WRITE_SINGLE_BYTE(sx1276_t *sx1276, uint8_t addr, uint8_t data)
{
    uint8_t txData[2];

    txData[0] = addr | WRITE_OPERATION; // ensure that Bit 7 is 1 aka write operation
    txData[1] = data;

    gpio_put(sx1276->_cs, 0); // start communication

    // Check if we can write data to the spi device
    if (spi_is_writable(sx1276->spi))
    {
        spi_write_blocking(sx1276->spi, txData, 2);
    }
    else
    {
        printf("not writeable\n");
    }

    gpio_put(sx1276->_cs, 1); // stop communication
}

/**
 * @brief Write data with len number of bytes to the register at addr
 *
 * @param sx1276
 * @param addr
 * @param data
 * @param len
 */
void SX1276_WRITE(sx1276_t *sx1276, uint8_t addr, const char *data, size_t len)
{
    uint8_t txData[1];
    txData[0] = addr | WRITE_OPERATION; // ensure that Bit 7 is 1 aka write operation

    gpio_put(sx1276->_cs, 0); // start communication

    // Check if we can write data to the spi device
    if (spi_is_writable(sx1276->spi))
    {
        spi_write_blocking(sx1276->spi, txData, 1);
        spi_write_blocking(sx1276->spi, data, len);
    }
    else
    {
        printf("not writeable\n");
    }

    gpio_put(sx1276->_cs, 1); // stop communication
}