#ifndef INTCODE_H
#define INTCODE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
using namespace std;

struct State {
  bool terminated;
  long pc, relative_base;
  unordered_map<long, long> memory;
  vector<long> output;

  State(unordered_map<long, long> memory)
    : terminated(false), pc(0), relative_base(0), memory(memory), output(vector<long>()) {
    }

  State(bool terminated, long pc, long relative_base, unordered_map<long, long> memory, vector<long> output)
    : terminated(terminated), pc(pc), relative_base(relative_base), memory(memory), output(output) {
    }
};

unordered_map<long, long> parse_input();
State run(State s, vector<long> inputs = vector<long>());

#endif
