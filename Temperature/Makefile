# Makefile for Intrepid I2C example 

CC=arm-linux-gcc
CFLAGS=-Wall -std=c99 -pedantic

PROGRAM=AD7415Test
OBJECTS=AD7415.c AD7415Test.c

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^              

OBJECT: $(OBJECTS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf core* *.o *.gch $(PROGRAM)
