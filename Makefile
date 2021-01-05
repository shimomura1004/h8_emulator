CPPC = c++

OBJS = main.o

TARGET = h8300h

CFLAGS = -Wall -I.
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
