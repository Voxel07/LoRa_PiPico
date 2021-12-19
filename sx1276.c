#include "sx1276.h"

sx1276_t *test;

void sx1276_init_spi(sx1276_t *sx1276, spi_inst_t *spi, uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t cs, uint8_t reset, uint8_t dio0, uint8_t dio1)
{
    printf("init SPI\n");
    //init spi
    sx1276->spi = spi;

    //init pins
    sx1276->mosi = mosi;
    sx1276->miso = miso;
    sx1276->sck = sck;
    sx1276->cs = cs;
    sx1276->reset = reset;
    sx1276->dio0 = dio0;
    sx1276->dio1 = dio1;

    //validate
    //spi_init((sx1276->spi == 0) ? spi0 : spi1, 10 * 1000 * 1000);
    spi_init(spi0, 10 * 1000 * 1000);
    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    //set Lora pins
    gpio_set_function(sx1276->mosi, GPIO_FUNC_SPI);
    gpio_set_function(sx1276->miso, GPIO_FUNC_SPI);
    gpio_set_function(sx1276->sck, GPIO_FUNC_SPI);

    //init gpios
    gpio_init(sx1276->reset);
    gpio_set_dir(sx1276->reset, GPIO_OUT);
    gpio_put(sx1276->reset, 1);
    gpio_pull_up(sx1276->reset);

    gpio_init(sx1276->cs);
    gpio_set_dir(sx1276->cs, GPIO_OUT);
    gpio_put(sx1276->cs, 1);

    gpio_init(sx1276->dio0);
    gpio_set_dir(sx1276->dio0, GPIO_IN);
    gpio_pull_up(sx1276->dio0);

    gpio_init(sx1276->dio1);
    gpio_set_dir(sx1276->dio1, GPIO_IN);
    gpio_pull_up(sx1276->dio1);
}

uint8_t SX1276_READ(sx1276_t *sx1276, uint8_t addr)
{

    gpio_put(sx1276->cs, 0); //start communication
    uint8_t read_data[1];
    // Check if we can write data to the spi device
    if (spi_is_writable(sx1276->spi))
    {
        spi_write_blocking(sx1276->spi, &addr, 1); //select the register
    }
    else
    {
        printf("not writeable\n");
    }

    spi_read_blocking(sx1276->spi, 0, read_data, 1); //read value of register

    gpio_put(sx1276->cs, 1); //stop communication

    return read_data[0];
}

void SX1276_WRITE(sx1276_t *sx1276, uint8_t addr, uint8_t value)
{

    uint8_t response = addr & WRITE_OPERATION;
    gpio_put(sx1276->cs, 0);
    // spi_read_blocking(sx1276->spi, 0, &response, 1);
    if (spi_is_writable(sx1276->spi))
    {
        // printf("Spi is writeable\n");
        spi_write_blocking(sx1276->spi, &response, 1);
    }
    else
    {
        printf("not writeable\n");
    }

    // printf("Response of addr = %d \n", response);
    response = value;
    if (spi_is_writable(sx1276->spi))
    {
        printf("Spi is writeable\n");
        spi_write_blocking(sx1276->spi, &response, 1);
    }
    else
    {
        printf("not writeable\n");
    }
    // printf("Response of value = %d\n", response);

    // spi_write_read_blocking(sx1276->spi, src, dst, 1);
    gpio_put(sx1276->cs, 1);
}