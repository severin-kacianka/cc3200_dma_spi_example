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

uint32_t transfer_status;

static void interrupt_handler()
{
	uint32_t status = MAP_SPIIntStatus(GSPI_BASE,true);
	MAP_SPIIntClear(GSPI_BASE,SPI_INT_EOW);
	SPICSDisable(GSPI_BASE);
	transfer_status = COMPLETE;

	if(!(status & SPI_INT_EOW) )
	{
		Message("Error: Unexpected SPI interrupt!\n\r");
		while(1){};
	}
}

void spi_transfer(uint8_t* tx, uint8_t* rx)
{
	transfer_status = STARTED;
	SetupTransfer(UDMA_CH30_GSPI_RX,UDMA_MODE_BASIC,DMA_SIZE,
                UDMA_SIZE_8,UDMA_ARB_1,
                (void *)(GSPI_BASE + MCSPI_O_RX0),UDMA_SRC_INC_NONE,
                rx,UDMA_DST_INC_8);

	SetupTransfer(UDMA_CH31_GSPI_TX,UDMA_MODE_BASIC,DMA_SIZE,
                UDMA_SIZE_8,UDMA_ARB_1,
                tx,UDMA_SRC_INC_8,
				(void *)(GSPI_BASE + MCSPI_O_TX0),UDMA_DST_INC_NONE);	
	SPICSEnable(GSPI_BASE);

	while(transfer_status != COMPLETE)
	{}
}

int main()
{
	uint32_t tx_crc = 0;
	uint32_t tx_crc_old = 0;
	uint32_t a = 364;
	uint32_t b = 53457;
	uint32_t c = 976;
	uint32_t d = 66;
	uint32_t error_counter = 0;
	uint32_t transfer_counter = 0;


	init();

	// initialize buffers with some known value
	memset(tx_buffer,0x33,sizeof(tx_buffer));
	memset(rx_buffer,0x44,sizeof(rx_buffer));

	// wait for keystroke: this gives the user time to start the slave
    Report("Start the slave and press any key to transmit data....\n\r");
    MAP_UARTCharGet(UARTA0_BASE);
	
	spi_setup(SPI_MODE_MASTER,interrupt_handler);

	tx_crc = fill_buffer(tx_buffer,DMA_SIZE);
	// One "dummy" transfer, to fill the slave's TX buffer
	// this gets rid of the first "wrong" transfer in which
	// we would only receive the slave's default buffer values
	spi_transfer(tx_buffer,rx_buffer);

	while(1)
	{
		tx_crc_old = tx_crc;
		change_seed(a++,b++,c++,d++);
		tx_crc = fill_buffer(tx_buffer,DMA_SIZE);
		
		spi_transfer(tx_buffer,rx_buffer);
		/*
		// uncomment for step-by-step transfers
		print_buffer(rx_buffer,DMA_SIZE,"RX");
		print_buffer(tx_buffer,DMA_SIZE,"TX");
		Report("error_counter: %d\n\r",error_counter);
		Report("Press any key to start the next transfer...\n\r");
		MAP_UARTCharGet(UARTA0_BASE);
		*/
		if(crc(rx_buffer,DMA_SIZE) != tx_crc_old)
		{
		//	Report("\n\rERROR\n\r");
			error_counter++;
		}
		transfer_counter++;
		if(transfer_counter == 1000)
		{
			Report("Transferred 1000 items, %d errors!\n\r",error_counter);
			error_counter = 0;
			transfer_counter = 0;
			Report("Press any key to test again\n\r");
			MAP_UARTCharGet(UARTA0_BASE);
		}
	}
	return 0;
}

