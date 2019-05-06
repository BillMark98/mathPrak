CXX := $(CXX) -std=c++11
CXXFLAGS = -g -Wall -fPIC

.PHONY: clean

SOURSE = vektor.o sparse_matrix.o A3.cpp unit.o
A3: vektor.o vektor.h sparse_matrix.o sparse_matrix.h A3.cpp unit.h unit.o
	$(CXX) $(CXXFLAGS) -o $@ $(SOURSE)

meine_loesung.o: #Hier fehlt was
	#Hier fehlt was

test_mv: vektor.o sparse_matrix.o test_mv.o
	$(CXX) $(CXXFLAGS) -o $@ vektor.o sparse_matrix.o test_mv.o
	
sparse_matrix.o: sparse_matrix.cpp sparse_matrix.h vektor.h
	$(CXX) $(CXXFLAGS) -c $<

vektor.o: vektor.cpp vektor.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f A3 meine_loesung.o vektor.o sparse_matrix.o

