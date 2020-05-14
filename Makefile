CXX = g++
CXXFLAGS = -std=c++11 -Wall

PROGS = day01 day02 day03 day04 day05 day06 day07 day08 day09 day10 day11 day12 day13 day14 day15 day16 day17 day18 day19 day20 day21 day22 day23 day24 day25
INTCODEPROGS = day02 day05 day07 day09 day11 day13 day15 day17 day19 day21 day23 day25

all: $(PROGS)

$(INTCODEPROGS): %: %.o intcode.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(INTCODEPROGS:=.o): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm *.o
