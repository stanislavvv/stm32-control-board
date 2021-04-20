######################################################################
#  Project Makefile
######################################################################
TOP_DIR         := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# choose target (stm32f103, stm32f303)
TARGET_CPU ?= stm32f103

TGT_CFLAGS	+= -I./rtos -I./libs -I.
TGT_CXXFLAGS	+= -I./rtos -I./libs -I.

BINARY		= main
SRCFILES	= rtos/heap_4.c rtos/list.c rtos/port.c rtos/tasks.c rtos/opencm3.c rtos/queue.c
SRCFILES	+= hw/spi.c hw/uart.c lcd/fonts.c lcd/st7789.c lcd/pcd8544.c lcd/hd44780.c hw.c 
SRCFILES	+= shell/shell.c shell/shell_cmds.c
SRCFILES	+= main.c

include mk/Makefile.common.incl

# tests
test: clean
	make -f Makefile.tests
	#make clean

# ci
ci: clean check all test docs
