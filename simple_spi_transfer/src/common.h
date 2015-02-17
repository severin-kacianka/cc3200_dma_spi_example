/*
* Copyright (c) 2015, Severin Kacianka (severin at kacianka dot at)
* All rights reserved.
* 
* Licensed under the BSD 2-Clause License
* See license.txt or http://opensource.org/licenses/BSD-2-Clause
*
* This work was done at the Alpen-Adria-University Klagenfurt (http://www.aau.at)
*/

// Standard includes
#include <string.h>
#include <stdint.h>


// Driverlib includes
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_ints.h"
#include "spi.h"
#include "rom.h"
#include "rom_map.h"
#include "utils.h"
#include "prcm.h"
#include "uart.h"
#include "interrupt.h"
#include "pin.h"
//for CRC
#include "crc.h"
#define CCM0_BASE           DTHE_BASE
// Common interface includes
#include "uart_if.h"

// 1MHz
//#define SPI_IF_BIT_RATE  800000
//#define SPI_IF_BIT_RATE  100000
#define SPI_IF_BIT_RATE  800000
#define TR_BUFF_SIZE     32000

void init(void);
void print_buffer(uint8_t* buffer, int len);
uint32_t crc(uint8_t* buffer);
uint32_t xorshift128(void);
void change_seed(uint32_t a, uint32_t b, uint32_t c, uint32_t d);
uint32_t fill_buffer(uint8_t* buffer, uint32_t len, char* name);
