/**
 * @file pico.h
 * @author your name (you@domain.com)
 * @brief This file contains Helper functions
 * @version 0.1
 * @date 2021-12-18
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef PICO_H
#define PICO_H

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include <ctype.h>

int printLoraPacket(uint8_t *hexValues, size_t length);

#endif