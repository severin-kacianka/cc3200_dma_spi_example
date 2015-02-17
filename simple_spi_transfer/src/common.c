/*
* Copyright (c) 2015, Severin Kacianka (severin at kacianka dot at)
* All rights reserved.
* 
* Licensed under the BSD 2-Clause License
* See license.txt or http://opensource.org/licenses/BSD-2-Clause
*
* This work was done at the Alpen-Adria-University Klagenfurt (http://www.aau.at)
*/

#include "common.h"
/* variables used for xorshift128 */
static uint32_t x = 5;
static uint32_t y = 8;
static uint32_t z = 32;
static uint32_t w = 8978;

void init()
{
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);
    PRCMCC3200MCUInit();

	// see pinmux.c in TI's SPI example 
    MAP_PRCMPeripheralClkEnable(PRCM_UARTA0, PRCM_RUN_MODE_CLK);
    MAP_PRCMPeripheralClkEnable(PRCM_GSPI, PRCM_RUN_MODE_CLK);
	MAP_PinTypeUART(PIN_55, PIN_MODE_3);
	MAP_PinTypeUART(PIN_57, PIN_MODE_3);
	MAP_PinTypeSPI(PIN_05, PIN_MODE_7);
	MAP_PinTypeSPI(PIN_06, PIN_MODE_7);
	MAP_PinTypeSPI(PIN_07, PIN_MODE_7);
	MAP_PinTypeSPI(PIN_08, PIN_MODE_7);

    MAP_PRCMPeripheralClkEnable(PRCM_GSPI,PRCM_RUN_MODE_CLK);
    InitTerm();
    ClearTerm();
    Message("\n\n\n\r");
    Message("\t\t   ********************************************\n\r");
    Message("\t\t        CC3200 SPI Demo Application!  \n\r");
    Report("\t\t        Birate:  %d  \n\r", SPI_IF_BIT_RATE);
    Report("\t\t        Buffersize:%d  \n\r", TR_BUFF_SIZE  );
    Message("\t\t   ********************************************\n\r");
    Message("\n\n\n\r");
    MAP_PRCMPeripheralReset(PRCM_GSPI);
	// for CRC
	MAP_PRCMPeripheralClkEnable(PRCM_DTHE, PRCM_RUN_MODE_CLK);
}

/*
* Prints a buffer
* if the buffer is bigger than 1 elements, 
* only the first 16 will be printed.
*/
void print_buffer(uint8_t* buffer, int len)
{
	uint32_t print_len = len;
	Report("Buffer");

	if(len > 16)
	{
		Message(" (16 first elements)");
		print_len = 16;
	}
	Message(":\n\r");
	for(uint32_t i = 0;i < print_len;i++)
	{
		Report("/%x",buffer[i]);
	}
	if(len > 16)
	{
		Report("/.../%x/\n\r",buffer[len-1]);
	}else{
		Report("/\n\r");
	}
	Report("Buffer's checksum is: 0x%02x\n\r", crc(buffer));
}

/*
* Perform CRC calculation
* See the crc example in the SDK for more details 
*/
uint32_t crc(uint8_t* buffer)
{
	uint32_t config = CRC_CFG_INIT_SEED | CRC_CFG_TYPE_P8005 | CRC_CFG_SIZE_8BIT;
	MAP_PRCMPeripheralReset(PRCM_DTHE);
	MAP_CRCConfigSet(CCM0_BASE, config);
	MAP_CRCSeedSet(CCM0_BASE, 0x0000ffff);
	return MAP_CRCDataProcess(CCM0_BASE, buffer, TR_BUFF_SIZE, config);
}

/*
* xorshift is a fast, but not too strong 
* random number generator 
* http://en.wikipedia.org/wiki/Xorshift
*/
uint32_t xorshift128(void) {
    uint32_t t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = w ^ (w >> 19) ^ t ^ (t >> 8);
}

void change_seed(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
	x = a;
	y = b;
	z = c;
	w = d;
}

uint32_t fill_buffer(uint8_t* buffer, uint32_t len, char* name)
{
	uint32_t tx_crc = 0;
	uint8_t tmp;
	uint32_t tx_sum;
	
	Message("----------------------------------\n\r");
	Report("Filling %s Buffer...\n\r",name);
	for(uint32_t i = 0; i < len; i++)
	{
		tmp = xorshift128();
		buffer[i] = tmp;
		tx_sum += tmp;
	}

	Report("Sum in the buffer is: %d\n\r", tx_sum);
	tx_crc = crc(buffer);
	Report("Buffer's checksum is: 0x%02x\n\r", tx_crc);
	Report("Filled %s Buffer!\n\r",name);
	Message("----------------------------------\n\r");
	return tx_crc;
}

