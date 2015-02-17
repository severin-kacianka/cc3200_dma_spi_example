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

void spi_transfer(uint8_t* tx, uint8_t* rx);

static void interrupt_handler()
{
	uint32_t status = MAP_SPIIntStatus(GSPI_BASE,true);
	MAP_SPIIntClear(GSPI_BASE,SPI_INT_EOW);

	spi_transfer(tx_buffer,rx_buffer);

	if(!(status &  SPI_INT_EOW))
	{
		Message("Error: Unexpected SPI interrupt!\n\r");
		while(1){};
	}
}

void spi_transfer(uint8_t* tx, uint8_t* rx)
{
	MAP_SPIDisable(GSPI_BASE);
	SetupTransfer(UDMA_CH30_GSPI_RX,UDMA_MODE_BASIC,DMA_SIZE,
                UDMA_SIZE_8,UDMA_ARB_1,
                (void *)(GSPI_BASE + MCSPI_O_RX0),UDMA_SRC_INC_NONE,
                rx,UDMA_DST_INC_8);

	SetupTransfer(UDMA_CH31_GSPI_TX,UDMA_MODE_BASIC,DMA_SIZE,
                UDMA_SIZE_8,UDMA_ARB_1,
                tx,UDMA_SRC_INC_8,
				(void *)(GSPI_BASE + MCSPI_O_TX0),UDMA_DST_INC_NONE);	
	MAP_SPIEnable(GSPI_BASE);
}

int main()
{

	init();
	// initialize buffers with some known value
	memset(tx_buffer,0x55,sizeof(tx_buffer));
	memset(rx_buffer,0x22,sizeof(rx_buffer));
	
	spi_setup(SPI_MODE_SLAVE,interrupt_handler);
	spi_transfer(tx_buffer,rx_buffer);

    Message("Enabled SPI Interface in Slave Mode!\n\r");

    while(1)
    {
		memcpy(tx_buffer,rx_buffer,DMA_SIZE);
		// here we could also change the tx_buffer
		// e.g.	tx_buffer[0] = 18;
    }
	return 0;
}

