CPPC = c++

OBJS = elf_loader.o inner_memory.o h8300h.o main.o
OBJS += instructions/adds.o instructions/movw.o instructions/movl.o instructions/jsr.o

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
