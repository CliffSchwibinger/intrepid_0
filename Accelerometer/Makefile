# Makefile for Intrepid example to use SPI1 bus in full-duplex mode. 

CC=arm-linux-gcc
CFLAGS+=-Wall -std=c99 -pedantic

PROGRAM=BMA250Test
OBJECTS=BMA250.c BMA250Test.c

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^              

OBJECT: $(OBJECTS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf core* *.o *.gch $(PROGRAM)
