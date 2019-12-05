CXX = g++
CXXFLAGS = -std=c++11 -Wall

two: two.o intcode.o
	$(CXX) $(CXXFLAGS) -o two two.o intcode.o

two.o: two.cpp intcode.h
	$(CXX) $(CXXFLAGS) -c two.cpp

three: three.cpp
	$(CXX) $(CXXFLAGS) -o three three.cpp

four: four.cpp
	$(CXX) $(CXXFLAGS) -o four four.cpp

five: five.o intcode.o
	$(CXX) $(CXXFLAGS) -o five five.o intcode.o

five.o: five.cpp intcode.h
	$(CXX) $(CXXFLAGS) -c five.cpp

intcode.o: intcode.h

clean:
	rm *.o
