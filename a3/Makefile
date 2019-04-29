CXXFLAGS = -O0 -g -Wall

main: main.cpp unit.h student.h student.o unit.o
	$(CXX) $(CXXFLAGS)	-o $@ main.cpp student.o unit.o
sort: sort.o unit.o student.o
	$(CXX) $(CXXFLAGS) -o $@ $^

sort.o: sort.cpp student.h
	$(CXX) $(CXXFLAGS) -c $<

student.o: student.cpp student.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f student.o sort.o sort

.PHONY: clean
