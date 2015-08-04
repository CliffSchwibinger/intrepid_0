# Makefile for HMC5883L 

CC=arm-linux-gcc
CFLAGS=-Wall -std=c99 -pedantic

PROGRAM=HMC5883LTest
OBJECTS=HMC5883L.c HMC5883LTest.c

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ -lm             

OBJECT: $(OBJECTS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf core* *.o *.gch $(PROGRAM)
