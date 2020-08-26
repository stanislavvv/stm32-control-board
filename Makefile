######################################################################
#  Project Makefile
######################################################################
TOP_DIR         := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# choose target (stm32f103, stm32f303)
TARGET_CPU ?= stm32f103

TGT_CFLAGS	+= -I./rtos -I.
TGT_CXXFLAGS	+= -I./rtos -I.

BINARY		= main
SRCFILES	= rtos/heap_4.c rtos/list.c rtos/port.c rtos/tasks.c rtos/opencm3.c
SRCFILES	+= st7789.c shell_hw.c shell_process.c hw.c shell.c main.c

include mk/Makefile.common.incl

# tests
test: clean
	make -f Makefile.tests
	make clean
