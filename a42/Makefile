CC = g++
CFLAGS = -g -Wall -std=c++11
CXXFLAGS = -std=c++11 -Wall -pedantic -Wextra -g -O3 -Ilib/SFML-2.5.1/include/ -fPIC
LIBS = -Llib/SFML-2.5.1/lib/ -lsfml-graphics -lsfml-window -lsfml-system

SOURCE = a_stern_debug.cpp mygraph.o unit.o mygraphvisual.o
SOURCE1 = mygraph.cpp



sfml_test: sfml_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)
sfml_test.o: sfml_test.cpp
	$(CXX) $(CXXFLAGS) -c $<
start: a_stern_debug.cpp mygraph.o mygraphvisual.o mygraph.h unit.o unit.h
	$(CC) $(CXXFLAGS) -o $@ $(SOURCE) $(LIBS)
mygraph.o: mygraph.cpp unit.o mygraph.h unit.h
	$(CC) $(CFLAGS) -c $(SOURCE1)
mygraphvisual.o:mygraphvisual.cpp mygraph.h mygraphvisual.h unit.h
	$(CC) $(CXXFLAGS) -c $<
clean:
	rm -f sfml_test.o sfml_test
run: sfml_test
	export LD_LIBRARY_PATH=./lib/SFML-2.5.1/lib && ./sfml_test
debug: sfml_test
	export LD_LIBRARY_PATH=./lib/SFML-2.5.1/lib && kdbg ./sfml_test
gdb: sfml_test
	gdb -ex "set environment LD_LIBRARY_PATH=./lib/SFML-2.5.1/lib" sfml_test
myopen: opengl.cpp
	g++ -o $@ $^ -lglut -lGLU -lGL
sfmltest: sfmltest.o
	g++ -o $@ $^ -lsfml-graphics -lsfml-window -lsfml-system
sfmltest.o: sfmltest.cpp
	g++ -c sfmltest.cpp
	
.PHONY: clean run debug gdb