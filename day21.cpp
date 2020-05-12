#include <iostream>
#include <vector>
#include "intcode.h"

using namespace std;

int main() {
  vector<long> inputs;
  auto memory = parse_input();
  State state = {memory};

  string input_string;
  input_string += "NOT A J\n";
  input_string += "NOT B T\n";
  input_string += "OR T J\n";
  input_string += "NOT C T\n";
  input_string += "OR T J\n";
  input_string += "AND D J\n";
  input_string += "WALK\n";

  for (char c : input_string) {
    inputs.push_back(c);
  }
  state = run(state, inputs);
  cout << "Part 1: " << state.output.back() << endl;
}
