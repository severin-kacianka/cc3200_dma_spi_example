# cc3200_dma_spi_example

Here are two examples that show how to use SPI with DMA and FIFO on the CC3200. I hope that they are a useful starting point for anyone, who wants to work on SPI and DMA in the future.

The examples are compiling fine on Linux with gcc 4.9.3 from http://launchpad.net/gcc-arm-embedded. All you need to change is the path for the SDK in the Makefile.
The hardware setup is similar to the SPI example in the SDK: just connect two CC3200 via cable (Pins GND, P05, P06, P07 and P08). I have set the following jumpers: J6, J7, J8. J9. J10, J11, J12, J13.

simple_spi_transfer is an example of an SPI transfer without DMA and FIFO. The master sends a buffer to the slave, reads it back in the next transfers and verifies the checksums.

transfer_1024_byte implements a simple SPI/DMA transfer that will transfer 1024 (or whatever DMA_SIZE is set to) bytes from the master to the slave and back. It then calculates the buffer's CRC checksum and (on the master) compares the send checksum to the received checksum.

transfer_64k shows how to transfer a 64k buffer from the master to the slave and back again.

windows contains a Makefile send to me by Matt van de Werken that also works
using the Launchpad tools in a Cygwin enviroment

# License
Copyright (c) 2015, Severin Kacianka (severin at kacianka dot at)
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
