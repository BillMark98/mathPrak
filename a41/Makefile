CC = g++
CFLAGS = -g -Wall -std=c++11
SOURCE = a_stern.cpp mygraph.o unit.o
SOURCE1 = mygraph.cpp
start: a_stern.cpp mygraph.o mygraph.h unit.o unit.h
	$(CC) $(CFLAGS) -o $@ $(SOURCE)

mygraph.o: mygraph.cpp unit.o mygraph.h unit.h 
	$(CC) $(CFLAGS) -c $(SOURCE1)