######################################################################
#  Project Makefile
######################################################################
TOP_DIR         := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# choose target (stm32f103, stm32f303)
TARGET_CPU ?= stm32f103

# process tests (make TARGET_CPU=test
ifeq ($(TARGET_CPU),test)
TGT_CFLAGS	+= -I.
TGT_CXXFLAGS	+= -I.

BINARY		= tests
SRCFILES	= tests.c shell_process.c

include mk/Makefile.tests.incl

# standard work
else
TGT_CFLAGS	+= -I./rtos -I.
TGT_CXXFLAGS	+= -I./rtos -I.

BINARY		= main
SRCFILES	= main.c rtos/heap_4.c rtos/list.c rtos/port.c rtos/tasks.c rtos/opencm3.c shell.c hw.c

include mk/Makefile.common.incl
endif
