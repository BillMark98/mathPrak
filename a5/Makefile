CC=g++
CFLAGS = -g -Wall -O0

source = mapraview.cpp greyscale.o unit.o

start: mapraview.cpp greyscale.o greyscale.h unit.o unit.h
	$(CC) $(CFLAGS) -o $@ $(source)
greyscale.o: greyscale.cpp greyscale.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
	rm *.o start