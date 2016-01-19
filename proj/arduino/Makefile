##############################################
# Quantenna Communications Inc               #
# (c) 2010 - All Rights Reserved             #
##############################################           
ARDUINO = 1
PROJ = .
ROOT = ../..
UTIL = $(ROOT)/util
DRIVER= $(ROOT)/driver
APPS= $(ROOT)/app
INCLUDE= -I. -I$(ROOT)/include
LIB= $(ROOT)/lib
LIBS += 

MAP_FILE=

TARGET= hello
SRC = \
	main.c \
	$(DRIVERS)/i2c.c \
	$(DRIVERS)/uart.c \
	
ASRC += 

CFLAGS +=  
LFLAGS += 
AFLAGS +=

all: obj $(TARGET)
	@echo Build completed


##############################################
# COMMON MAKEFILE                            #
##############################################           
include $(ROOT)/include/makefile.mk

clean:
	rm -rf obj $(TARGET).*

