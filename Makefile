CXX = g++
CXXFLAGS = -std=c++11 -Wall

PROGS = 1a 1b two three four five six seven eight nine ten eleven twelve thirteen fourteen fifteen sixteen seventeen eighteen nineteen twenty
INTCODEPROGS = two five seven nine eleven thirteen fifteen seventeen nineteen

all: $(PROGS)

$(INTCODEPROGS): %: %.o intcode.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(INTCODEPROGS:=.o): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm *.o
