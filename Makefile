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

six: six.cpp
	$(CXX) $(CXXFLAGS) -o six six.cpp

seven: seven.o intcode.o
	$(CXX) $(CXXFLAGS) -o seven seven.o intcode.o

seven.o: seven.cpp intcode.h
	$(CXX) $(CXXFLAGS) -c seven.cpp

nine: nine.o intcode.o
	$(CXX) $(CXXFLAGS) -o nine nine.o intcode.o

nine.o: nine.cpp intcode.h
	$(CXX) $(CXXFLAGS) -c nine.cpp

ten: ten.cpp
	$(CXX) $(CXXFLAGS) -o ten ten.cpp

intcode.o: intcode.h

clean:
	rm *.o
