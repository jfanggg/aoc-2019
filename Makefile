CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

two: two.o intcode.o
	$(CXX) $(CXXFLAGS) -o two two.o intcode.o

two.o: two.cpp intcode.h
	$(CXX) $(CXXFLAGS) -c two.cpp

intcode.o: intcode.h

clean:
	rm *.o
