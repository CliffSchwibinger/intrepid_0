# Makefile for Intrepid I2C example 

CC=arm-linux-gcc
CFLAGS=-Wall -std=c99 -pedantic

PROGRAM=L3G4200DTest
OBJECTS=L3G4200D.c L3G4200DTest.c

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^              

OBJECT: $(OBJECTS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf core* *.o *.gch $(PROGRAM)
