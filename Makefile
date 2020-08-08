######################################################################
#  Project Makefile
######################################################################
TOP_DIR         := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

TGT_CFLAGS	+= -I./rtos -I.
TGT_CXXFLAGS	+= -I./rtos -I.

BINARY		= main
SRCFILES	= main.c rtos/heap_4.c rtos/list.c rtos/port.c rtos/tasks.c rtos/opencm3.c morse.c

include mk/Makefile.common.incl
# choose target
include mk/Makefile.stm32f103.incl
#include mk/Makefile.stm32f303.incl
