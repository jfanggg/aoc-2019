#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include "intcode.h"

using namespace std;

int main() {
  vector<long> inputs;
  auto memory = parse_input();

  State state(memory);
  inputs = vector<long>({1});
  state = run(state, inputs);
  cout << "Part 1: " << state.output[state.output.size() - 1] << endl;

  state = State(memory);
  inputs = vector<long>({5});
  state = run(state, inputs);
  cout << "Part 2: " << state.output[0] << endl;
}
