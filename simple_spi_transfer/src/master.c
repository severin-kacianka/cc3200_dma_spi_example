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
static uint8_t tx_buffer[TR_BUFF_SIZE];
static uint8_t rx_buffer[TR_BUFF_SIZE];

int main()
{
	uint32_t rx_sum = 0;
	uint32_t tx_crc = 0;
	uint32_t tx_crc_old = 0;
	uint32_t a = 364;
	uint32_t b = 53457;
	uint32_t c = 976;
	uint32_t d = 66;


	init();
	// initialize buffers with some known value
	memset(tx_buffer,0x33,sizeof(tx_buffer));
	memset(rx_buffer,0x44,sizeof(rx_buffer));

	// wait for keystroke: this gives the user time to start the slave
    Report("Start the slave and press any key to transmit data....\n\r");
    MAP_UARTCharGet(UARTA0_BASE);
	
	// configure SPI channel
	MAP_SPIReset(GSPI_BASE);
    MAP_SPIConfigSetExpClk(GSPI_BASE,MAP_PRCMPeripheralClockGet(PRCM_GSPI),
                     SPI_IF_BIT_RATE,SPI_MODE_MASTER,SPI_SUB_MODE_0,
                     (SPI_SW_CTRL_CS |
                     SPI_4PIN_MODE |
                     SPI_TURBO_OFF |
                     SPI_CS_ACTIVEHIGH |
                     SPI_WL_8));	
	MAP_SPIEnable(GSPI_BASE);
	
	tx_crc = fill_buffer(tx_buffer,TR_BUFF_SIZE,"TX");

	while(1)
	{
		tx_crc_old = tx_crc;
		change_seed(a++,b++,c++,d++);
		tx_crc = fill_buffer(tx_buffer,TR_BUFF_SIZE,"TX");
		Message("Sending...");

		SPICSEnable(GSPI_BASE);
		for(int i = 0; i < TR_BUFF_SIZE; i++)
		{
			SPIDataPut(GSPI_BASE,tx_buffer[i]);
			SPIDataGet(GSPI_BASE,&rx_buffer[i]);
		}
		Message("Transfer complete!\n\r");
		SPICSDisable(GSPI_BASE);
		// we could also use SPITransfer instead:
		// MAP_SPITransfer(GSPI_BASE,tx_buffer,rx_buffer, TR_BUFF_SIZE,
		//		SPI_CS_ENABLE|SPI_CS_DISABLE);

		rx_sum = 0;
		for(int i = 0; i < TR_BUFF_SIZE; i++)
		{
			rx_sum += rx_buffer[i];
		}
		Report("\n\rChecksum in the RX buffer was: 0x%02x, we send 0x%02x\n\r\n\r",crc(rx_buffer),tx_crc_old);
		Message("TX-");
		print_buffer(tx_buffer,TR_BUFF_SIZE);
		Message("RX-");
		print_buffer(rx_buffer,TR_BUFF_SIZE);		
		Report("Press any key to transmit again\n\r");
		MAP_UARTCharGet(UARTA0_BASE);
	}
	return 0;
}

