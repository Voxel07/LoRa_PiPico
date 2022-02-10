#include "pico.h"

int printLoraPacket(uint8_t *hexValues, size_t length)
{
    // Ensure that Letters in the hex string are upper case
    uint8_t str[length];

    for (size_t i = 0; i < length; i++)
    {
        str[i] = toupper(hexValues[i]);
    }

    for (size_t i = 0; i < length; i++)
    {
        printf("%c", str[i]);
    }

    printf("\n");
}
