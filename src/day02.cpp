#include <iostream>
#include <unordered_map>
#include <cstdlib>
#include "intcode.h"
using namespace std;

int main() {
  auto memory = parse_input();

  // Part 1
  memory[1] = 12;
  memory[2] = 2;
  State state(memory);
  state = run(state);
  cout << "Part 1: " << state.memory.at(0) << endl;

  // Part 2
  for (int n = 0; n < 100; n++) {
    for (int v = 0; v < 100; v++) {
      memory[1] = n;
      memory[2] = v;

      state = State(memory);
      state = run(state);
      if (state.memory.at(0) == 19690720) {
        cout << "Part 2: " << 100*n + v << endl;
      }
    }
  }
}

