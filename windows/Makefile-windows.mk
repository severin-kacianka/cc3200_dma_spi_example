# This Makefile was send to me by Matt van de Werken (a big thanks to you!)
# He had trouble using my (Linux-only-tested) Makefiles on Windows with the 
# Launchpad tools in an Cygwin environment and this Makefile fixed it for him 

#
# The base directory for TivaWare.
#
SDKROOT=../../cc3200-sdk

#
# Include the common make definitions.
#
include ${SDKROOT}/tools/gcc_scripts/makedefs

#
# Where to find source files that do not live in this directory.
#
VPATH=./src
VPATH+=${SDKROOT}/example/common
VPATH+=${SDKROOT}/driverlib

#
# Where to find header files that do not live in the source directory.
#
IPATH=./src
IPATH+=${SDKROOT}
IPATH+=${SDKROOT}/inc
IPATH+=${SDKROOT}/driverlib
IPATH+=${SDKROOT}/example/common

#
# The default rule, which causes the driver library to be built.
#
all: ${OBJDIR} ${BINDIR}
all: ${BINDIR}/master.axf
all: ${BINDIR}/slave.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${OBJDIR} ${wildcard *~}
	@rm -rf ${BINDIR} ${wildcard *~}


#
# The rule to create the target directories.
#
${OBJDIR}:
	@mkdir -p ${OBJDIR}

${BINDIR}:
	@mkdir -p ${BINDIR}

#
# Rules for building the example.
#

${BINDIR}/slave.axf: ${OBJDIR}/slave.o
${BINDIR}/slave.axf: ${OBJDIR}/common.o
${BINDIR}/slave.axf: ${OBJDIR}/uart_if.o
${BINDIR}/slave.axf: ${OBJDIR}/udma_if.o
${BINDIR}/slave.axf: ${OBJDIR}/startup_${COMPILER}.o
${BINDIR}/slave.axf: ${SDKROOT}/driverlib/${COMPILER}/${BINDIR}/libdriver.a

${BINDIR}/master.axf: ${OBJDIR}/master.o
${BINDIR}/master.axf: ${OBJDIR}/common.o
${BINDIR}/master.axf: ${OBJDIR}/uart_if.o
${BINDIR}/master.axf: ${OBJDIR}/udma_if.o
${BINDIR}/master.axf: ${OBJDIR}/startup_${COMPILER}.o
${BINDIR}/master.axf: ${SDKROOT}/driverlib/${COMPILER}/${BINDIR}/libdriver.a

SCATTERgcc_slave=config/memory_layout.ld
ENTRY_slave=ResetISR

SCATTERgcc_master=config/memory_layout.ld
ENTRY_master=ResetISR


#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${OBJDIR}/*.d} __dummy__
endif
