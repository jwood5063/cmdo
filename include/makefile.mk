##############################################
# Silicon bringup makefile
# Common definitions
##############################################

##############################################
#  Tool defs
##############################################
ifdef ARDUINO
AVRDUDE=avrdude -F -V
OBJCOPY=avr-objcopy
CC=avr-gcc
RM=rm -f
MCU=atmega2560
F_CPU=16000000UL
BIN_FORMAT=ihex
#PORT=/dev/cuaU0
PORT=/dev/ttyACM0
BAUD=115200
PROTOCOL=wiring
PART=ATMEGA2560
CFLAGS=-std=gnu99 -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) $(INCLUDE) 
SUDO=sudo

endif

ifdef PI
CC=gcc
AR=ar
AS=$(CC)
LD=$(CC)
endif

MAKE_DEPEND=$(ROOT)/bin/makedepend

##############################################
# Flags
##############################################
LFLAGS += 
CFLAGS += -I$(INCLUDE) -I$(PROJ)
AFLAGS += 

OBJS = $(addprefix obj/, $(notdir $(SRC:.c=.o)))
AOBJS = $(addprefix obj/, $(notdir $(ASRC:.s=.o)))

DEPENDS = $(addprefix obj/, $(notdir $(SRC:.c=.u)))
##############################################
# Rules
##############################################
default: all
	echo done

obj:
	if [ ! -d obj ]; then mkdir obj; fi

ifdef ARDUINO
$(TARGET).elf: $(OBJS) $(AOBJS)
	$(CC) $(CFLAGS) $< -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O $(BIN_FORMAT) -R .eeprom $< $@

$(TARGET): $(TARGET).hex

endif

ifdef PI
$(TARGET): $(OBJS) $(AOBJS)
	$(CC) $(OBJS) $(AOBJS) $(CFLAGS) -o $(TARGET)
endif
obj/%.o: $(DRIVER)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

obj/%.o: $(UTIL)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

obj/%.o: $(PROJ)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: cscope
cscope:
	cscope -b $(INCLUDE) $(SRC)
	
.PHONY: upload
upload: ${PROG}.hex
	${SUDO} $(AVRDUDE) -c $(PROTOCOL) -p $(PART) -P $(PORT) \
		-b $(BAUD) -U flash:w:${PROG}.hex
.PRECIOUS: obj/%.o
	
