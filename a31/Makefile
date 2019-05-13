CXX := $(CXX) -std=c++11
CXXFLAGS = -g -Wall -fPIC

.PHONY: clean

SOURSE = meine_loesung.o vektor.o sparse_matrix.o unit.o

A3: vektor.o vektor.h sparse_matrix.o sparse_matrix.h meine_loesung.o unit.h unit.o
	$(CXX) $(CXXFLAGS) -o $@ $(SOURSE)
meine_loesung.o: meine_loesung.cpp	
	$(CXX) $(CXXFLAGS) -c $<

test_mv: vektor.o sparse_matrix.o test_mv.o
	$(CXX) $(CXXFLAGS) -o $@ vektor.o sparse_matrix.o test_mv.o
	
sparse_matrix.o: sparse_matrix.cpp sparse_matrix.h vektor.h
	$(CXX) $(CXXFLAGS) -c $<

vektor.o: vektor.cpp vektor.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f A3 meine_loesung.o vektor.o sparse_matrix.o

