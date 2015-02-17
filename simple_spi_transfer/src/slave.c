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
static uint32_t interrupt_count;
static uint32_t transfer_count;
static uint32_t tx_count;
static uint32_t rx_count;
static uint8_t tx_buffer[TR_BUFF_SIZE];
static uint8_t rx_buffer[TR_BUFF_SIZE];

static void interrupt_handler()
{
    uint32_t recv_data;
    uint32_t status;

    status = MAP_SPIIntStatus(GSPI_BASE,true);
    MAP_SPIIntClear(GSPI_BASE,SPI_INT_RX_FULL|SPI_INT_TX_EMPTY);

	if(status & SPI_INT_TX_EMPTY)
    {
		MAP_SPIDataPut(GSPI_BASE,tx_buffer[tx_count % TR_BUFF_SIZE]);
		tx_count++;
	}

	if(status & SPI_INT_RX_FULL)
	{
		MAP_SPIDataGetNonBlocking(GSPI_BASE,&recv_data);
		rx_buffer[rx_count % TR_BUFF_SIZE] = recv_data;
		rx_count++;
	}
	interrupt_count++;
	if(interrupt_count % (TR_BUFF_SIZE * 2) == 0 && interrupt_count > 0)
	{
		transfer_count++;
	}
}

int main()
{
	interrupt_count = 0;
	transfer_count = 0;
	tx_count = 0;
	rx_count = 0;

	uint32_t output_count = 0;
	uint32_t sum = 0;
	
	
	init();
	// initialize buffers with some known value
	memset(tx_buffer,0x55,sizeof(tx_buffer));
	memset(rx_buffer,0x22,sizeof(rx_buffer));
	
	// configure SPI
	MAP_SPIReset(GSPI_BASE);
    MAP_SPIConfigSetExpClk(GSPI_BASE,MAP_PRCMPeripheralClockGet(PRCM_GSPI),
                     SPI_IF_BIT_RATE,SPI_MODE_SLAVE,SPI_SUB_MODE_0,
                     (SPI_HW_CTRL_CS |
                     SPI_4PIN_MODE |
                     SPI_TURBO_OFF |
                     SPI_CS_ACTIVEHIGH |
                     SPI_WL_8));	
	MAP_SPIIntRegister(GSPI_BASE,interrupt_handler);
	MAP_SPIIntEnable(GSPI_BASE,SPI_INT_RX_FULL|SPI_INT_TX_EMPTY);
	MAP_SPIEnable(GSPI_BASE);
    Message("Enabled SPI Interface in Slave Mode!\n\r");

	Message("Starting while\n\r");
    while(1)
    {

		memcpy(tx_buffer,rx_buffer,TR_BUFF_SIZE);
		// here we could also change the tx_buffer
		// e.g.	tx_buffer[TR_BUFF_SIZE - 1] = 18;
		sum = 0;
		if(output_count < transfer_count)
		{
			for(int i = 0; i < TR_BUFF_SIZE; i++)
			{
				sum += rx_buffer[i];
			}
			Report("The sum in the Rx buffer is: %d\n\r",sum);
			Report("Checksum Rx buffer is: 0x%02x\n\r",crc(rx_buffer));
			Report("interrupt: %d, tx: %d, rx: %d, transfer: %d\n\r",interrupt_count,tx_count,rx_count, transfer_count);
			Message("TX-");
			print_buffer(tx_buffer,TR_BUFF_SIZE);
			Message("RX-");
			print_buffer(rx_buffer,TR_BUFF_SIZE);
			output_count++;
		}

    }
	return 0;
}

