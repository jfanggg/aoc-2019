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

  int x = 3, y = 4;
  int top_left = 0, top_right = 0;

  while (!(top_left == 1 && top_right == 1)) {
    x++;
    y++;

    state = run({memory}, {x, y});
    while (state.output.at(0) == 0) {
      state = run({memory}, {x, ++y});
    }

    if (x >= 99) {
      state = run({memory}, {x - 99, y});
      top_left = state.output.at(0);
      state = run({memory}, {x - 99, y + 99});
      top_right = state.output.at(0);
    }
  }
  cout << "Part 2: " << 10000 * (x - 99) + y << endl;
}
