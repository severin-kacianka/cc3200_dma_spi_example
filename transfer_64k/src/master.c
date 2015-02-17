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
	MAP_SPICSDisable(GSPI_BASE);
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
	MAP_SPICSEnable(GSPI_BASE);

	while(transfer_status != COMPLETE)
	{}
}

int main()
{
	uint32_t data_buffer_crc = 0;
	uint32_t transfer_counter = 0;
	uint32_t transfer_mode = RX;

	init();

	// initialize buffers with some known value
	memset(tx_buffer,0x33,sizeof(tx_buffer));
	memset(rx_buffer,0x44,sizeof(rx_buffer));

	// wait for keystroke: this gives the user time to start the slave
    Report("Start the slave and press any key to transmit data....\n\r");
    MAP_UARTCharGet(UARTA0_BASE);
	
	spi_setup(SPI_MODE_MASTER,interrupt_handler);

	data_buffer_crc = fill_buffer(data_buffer,BUFFER_SIZE);
	print_buffer(data_buffer,BUFFER_SIZE,"data");
	Report("data_buffer CRC is : %x\n\r",data_buffer_crc);

	while(1)
	{
		if(transfer_counter == 0)
		{
			Report("Transferred data_buffer!\n\r");
			Report("Press any key to test again again\n\r");
			MAP_UARTCharGet(UARTA0_BASE);
			if(transfer_mode == RX)
			{
				Report("RX CRC is : %x\n\r",crc(data_buffer,BUFFER_SIZE));
			}
			if(transfer_mode == TX)
			{
				// clear buffer to make sure we actually get the values
				// from the slave
				Message("Wiping buffer!\n\r");
				memset(data_buffer,0,BUFFER_SIZE);
			}
			transfer_mode = !transfer_mode;
		}

		if(transfer_mode == TX)
		{
			spi_transfer(data_buffer + transfer_counter,rx_buffer);
		} 
		if(transfer_mode == RX)
		{
			spi_transfer(tx_buffer,data_buffer + transfer_counter);
		}
		transfer_counter = (transfer_counter + DMA_SIZE) % BUFFER_SIZE;
	}
	return 0;
}

