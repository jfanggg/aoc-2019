#include <iostream>
#include <vector>
#include "intcode.h"

using namespace std;

int main() {
  vector<long> inputs;
  auto memory = parse_input();
  State state;

  int affected = 0;
  for (int i = 0; i < 50; i++) {
    for (int j = 0; j < 50; j++) {
      state = run({memory}, {i, j});
      affected += state.output.at(0);
    }
  }
  cout << "Part 1: " << affected << endl;
}
