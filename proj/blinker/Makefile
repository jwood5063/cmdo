##############################################
# Jim Wood
# (c) 2016 - All Rights Reserved             #
##############################################           
PI = 1
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

