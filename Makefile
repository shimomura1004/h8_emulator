CPPC = c++

OBJS = elf_loader.o inner_memory.o h8300h.o main.o
OBJS += operation_map/operation_map.o \
		operation_map/operation_map_0x.o \
		operation_map/operation_map_1x.o \
		operation_map/operation_map_4x.o \
		operation_map/operation_map_5x.o \
		operation_map/operation_map_6x.o \
		operation_map/operation_map_7Cx.o \
		operation_map/operation_map_7Dx.o \
		operation_map/operation_map_7Ex.o \
		operation_map/operation_map_7Fx.o \
		operation_map/operation_map_7x.o
OBJS += instructions/mov.o \
		instructions/jsr.o \
		instructions/exts.o \
		instructions/shll.o \
		instructions/sub.o \
		instructions/rts.o \
		instructions/bcc.o

TARGET = h8300h

CFLAGS = -Wall -I.
CFLAGS += -g
LFLAGS = -L.

.SUFFIXES: .c .o
.SUFFIXES: .cc .o
.SUFFIXES: .s .o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CPPC) $(OBJS) -o $(TARGET) $(CFLAGS) $(LFLAGS)

.c.o :			$<
				$(CPPC) -c $(CFLAGS) $<

.s.o :			$<
				$(CPPC) -c $(CFLAGS) $<

clean:
	rm -f $(OBJS) $(TARGET)
