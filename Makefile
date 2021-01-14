CC = cc
CPPC = c++

OBJS = elf_loader.o inner_memory.o h8300h.o runner.o main.o
OBJS += sci/sci.o sci/sci_register.o
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
		instructions/bcc.o \
		instructions/cmp.o \
		instructions/subs.o \
		instructions/and.o \
		instructions/adds.o \
		instructions/add.o \
		instructions/shlr.o \
		instructions/dec.o \
		instructions/extu.o \
		instructions/bclr.o \
		instructions/btst.o \
		instructions/neg.o \
		instructions/xor.o \
		instructions/shar.o \
		instructions/mulxs.o \
		instructions/nop.o \
		instructions/orc.o \
		instructions/bset.o \
		instructions/andc.o \
		instructions/rte.o

TARGET = h8300h

CFLAGS = -Wall -I. -std=c++11 -pthread
CFLAGS += -g
LFLAGS = -L.

.SUFFIXES: .cc .c .o

all: $(TARGET) sender

$(TARGET): $(OBJS)
	$(CPPC) $(OBJS) -o $(TARGET) $(CFLAGS) $(LFLAGS)

sender: sender.c
	$(CC) -o sender sender.c

.cc.o :			$<
				$(CPPC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
