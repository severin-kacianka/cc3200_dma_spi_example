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
// for CRC
#include "crc.h"
#define CCM0_BASE           DTHE_BASE
// Common interface includes
#include "uart_if.h"
#include "pin.h"
// for DMA
#include "udma.h"
#include "udma_if.h"
#include "hw_mcspi.h"

/* SPI_IF_BIT_RATE must be an integer divisor of 20000000 */
#define SPI_IF_BIT_RATE		20000000 / 2
/* Valid range: 1 - 1024 */
#define DMA_SIZE			1024
#define STARTED  0
#define COMPLETE 1

void init(void);
void spi_setup(uint32_t mode,void(*interrupt_handler)(void));
void print_buffer(uint8_t* buffer, uint32_t len, char* name);
uint32_t crc(uint8_t* buffer, uint32_t size);
uint32_t xorshift128(void);
void change_seed(uint32_t a, uint32_t b, uint32_t c, uint32_t d);
uint32_t fill_buffer(uint8_t* buffer, uint32_t len);

static uint8_t tx_buffer[DMA_SIZE];
static uint8_t rx_buffer[DMA_SIZE];
