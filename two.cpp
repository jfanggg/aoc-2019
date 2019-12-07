#include <iostream>
#include <vector>
#include <cstdlib>
#include "intcode.h"
using namespace std;

int main() {
  string delimiter = ",";
  string s;

  vector<int> program;
  int x;
  size_t start = 0, end = 0;

  cin >> s;
  while ((end = s.find(delimiter, start)) != std::string::npos) {
    x = atoi(s.substr(start, end).c_str());
    program.push_back(x);
    start = end + 1;
  }
  x = atoi(s.substr(start).c_str());
  program.push_back(x);

  // Part 1
  program[1] = 12;
  program[2] = 2;
  State state(program);
  state = run(state);
  cout << "Part 1: " << state.program[0] << endl;

  // Part 2
  for (int n = 0; n < 100; n++) {
    for (int v = 0; v < 100; v++) {
      program[1] = n;
      program[2] = v;

      state = State(program);
      state = run(state);
      if (state.program[0] == 19690720) {
        cout << "Part 2: " << 100*n + v << endl;
      }
    }
  }
}

