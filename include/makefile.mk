##############################################
# Silicon bringup makefile
# Common definitions
##############################################

##############################################
#  Tool defs
##############################################
CC=gcc
AR=ar
AS=$(CC)
LD=$(CC)
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

$(TARGET): $(OBJS) $(AOBJS)
	$(CC) $(OBJS) $(AOBJS) -o $(TARGET)

obj/%.o: $(DRIVER)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

obj/%.o: $(UTIL)/%.c
	$(CC) -c $(CFLAGS) $< -o $@
	
obj/%.o: $(PROJ)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

cscope:
	cscope -b $(INCLUDE) $(SRC)

.PRECIOUS: obj/%.o
	
