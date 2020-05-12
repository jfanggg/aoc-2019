#include <iostream>
#include <vector>
#include "intcode.h"

using namespace std;

int main() {
  vector<long> inputs;
  auto memory = parse_input();
  State state0 = {memory};

  string input_string1;
  input_string1 += "NOT A J\n";
  input_string1 += "NOT B T\n";
  input_string1 += "OR T J\n";
  input_string1 += "NOT C T\n";
  input_string1 += "OR T J\n";
  input_string1 += "AND D J\n"; // ((-A or -B or -C) and D)
  input_string1 += "WALK\n";

  for (char c : input_string1) {
    inputs.push_back(c);
  }
  State state1 = run(state0, inputs);
  cout << "Part 1: " << state1.output.back() << endl;

  string input_string2;
  input_string2 += "NOT A J\n";
  input_string2 += "NOT B T\n";
  input_string2 += "OR T J\n";  
  input_string2 += "NOT C T\n";
  input_string2 += "OR T J\n";  
  input_string2 += "AND D J\n"; // ((-A or -B or -C) and D)
  input_string2 += "NOT E T\n"; 
  input_string2 += "NOT T T\n"; 
  input_string2 += "OR H T\n"; 
  input_string2 += "AND T J\n"; // (-A or -B or -C) and D and (E or H)
  input_string2 += "RUN\n";

  inputs.clear();
  for (char c : input_string2) {
    inputs.push_back(c);
  }
  State state2 = run(state0, inputs);
  cout << "Part 2: " << state2.output.back() << endl;
}
