# Copyright (c) 2015, Severin Kacianka (severin at kacianka dot at)
# All rights reserved.
# 
# Licensed under the BSD 2-Clause License
# See license.txt or http://opensource.org/licenses/BSD-2-Clause
#
# This work was done at the Alpen-Adria-University Klagenfurt (http://www.aau.at)

# This Makefile is kept as simple and 'manual' as possible. It should
# be used as a reference and starting point for more 'automagic' Makefiles.
# You might want to run 'make -n' in the SDK examples and compare the output

SDK=${HOME}/CC3200_SDK/cc3200-sdk
SRC=./src
BUILD=./obj
BIN=./bin
MEM_LAYOUT=config/memory_layout.ld

CC=arm-none-eabi-gcc
CFLAGS=-mthumb -mcpu=cortex-m4  -ffunction-sections -fdata-sections -MD -std=c99 -g -O0 -Wall -Wextra -Wpedantic 
LD=arm-none-eabi-ld
LDFLAGS=-T ${MEM_LAYOUT} --entry ResetISR --gc-sections
OC=arm-none-eabi-objcopy 
MACROS=-DUSER_INPUT_ENABLE -Dgcc

INCLUDE=-I ${SRC}
INCLUDE+=-I ${SDK}/example
INCLUDE+=-I ${SDK}/example/common
INCLUDE+=-I ${SDK}/inc
INCLUDE+=-I ${SDK}/oslib
INCLUDE+=-I ${SDK}/driverlib

# Note that the order of the libs is important
LIBS=${SDK}/driverlib/gcc/exe/libdriver.a 
LIBS+=$(shell ${CC} ${CFLAGS} -print-file-name=libm.a)
LIBS+=$(shell ${CC} ${CFLAGS} -print-file-name=libc.a)
LIBS+=$(shell ${CC} ${CFLAGS} -print-libgcc-file-name)

all:  
	mkdir -p ${BUILD}
	mkdir -p ${BIN}
	${CC} ${CFLAGS} -c ${INCLUDE} ${MACROS} -o ${BUILD}/master.o ${SRC}/master.c 
	${CC} ${CFLAGS} -c ${INCLUDE} ${MACROS} -o ${BUILD}/slave.o ${SRC}/slave.c 
	${CC} ${CFLAGS} -c ${INCLUDE} ${MACROS} -o ${BUILD}/common.o ${SRC}/common.c 
	${CC} ${CFLAGS} -c ${INCLUDE} ${MACROS} -o ${BUILD}/uart_if.o ${SDK}/example/common/uart_if.c
	${CC} ${CFLAGS} -c ${INCLUDE} ${MACROS} -o ${BUILD}/udma_if.o ${SDK}/example/common/udma_if.c
	${CC} ${CFLAGS} -c ${INCLUDE} ${MACROS} -o ${BUILD}/startup_gcc.o ${SDK}/example/common/startup_gcc.c
	${LD} ${LDFLAGS} -o ${BIN}/master.axf ${BUILD}/master.o  ${BUILD}/common.o ${BUILD}/uart_if.o \
						${BUILD}/udma_if.o ${BUILD}/startup_gcc.o \
						${LIBS}
	${LD} ${LDFLAGS} -o ${BIN}/slave.axf ${BUILD}/slave.o ${BUILD}/common.o ${BUILD}/uart_if.o \
						${BUILD}/udma_if.o ${BUILD}/startup_gcc.o \
						${LIBS}
	$(OC) -O binary  ${BIN}/master.axf ${BIN}/master.bin
	$(OC) -O binary  ${BIN}/slave.axf ${BIN}/slave.bin
	@echo
	@echo "You can now run the program (maybe need to be root):"
	@echo "arm-none-eabi-gdb -x config/gdbinit bin/master.axf"
	@echo "arm-none-eabi-gdb -x config/gdbinit bin/slave.axf"


clean:
	rm -rf ${BUILD}
	rm -rf ${BIN}
