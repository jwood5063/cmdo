##############################################
# Quantenna Communications Inc               #
# (c) 2010 - All Rights Reserved             #
##############################################           
PROJ = .
ROOT = ../..
UTIL = $(ROOT)/util
DRIVER= $(ROOT)/driver
APPS= $(ROOT)/app
INCLUDE= -I. -I$(ROOT)/include
LIB= $(ROOT)/lib
LIBS += 

MAP_FILE=

TARGET= blinker

SRC = \
	main.c \
	$(DRIVERS)/bcm2835.c \
	
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
	rm -rf obj $(TARGET)

