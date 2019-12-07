#ifndef INTCODE_H
#define INTCODE_H

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct State {
  bool terminated;
  int idx;
  vector<int> program, output;

  State(vector<int> program_) {
    terminated = false;
    idx = 0;
    program = program_;
    output = vector<int>();
  }

  State(bool terminated_, int idx_, vector<int> program_, vector<int> output_) {
    terminated = terminated_;
    idx = idx_;
    program = program_;
    output = output_;
  }
};

State run(State s, vector<int> inputs = vector<int>());

#endif
