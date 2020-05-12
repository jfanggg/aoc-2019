#include <algorithm>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include "intcode.h"
using namespace std;

char ctoi(int x) {
  return x - '0';
}

int main() {
  auto memory = parse_input();

  int ans1 = 0;
  string settings = "01234";

  do {
    int x = 0;
    for (int i = 0; i < 5; i++) {
      x = run(State(memory), {ctoi(settings[i]), x}).output[0];
    }
    ans1 = max(ans1, x);
  } while(next_permutation(settings.begin(), settings.end()));
  cout << "Part 1: " << ans1 << endl;

  settings = "56789";
  int ans2 = 0;
  do {
    int x = 0;
    vector<State> states(5, State(memory));

    // pass in phase value first
    for (int i = 0; i < 5; i++) {
      states[i] = run(states[i], {ctoi(settings[i])});
    }
    // then actually do feedback loop
    while (!states[4].terminated) {
      for (int i = 0; i < 5; i++) {
        states[i] = run(states[i], {x});
        x = states[i].output.back();
      }
    }
    ans2 = max(ans2, x);
  } while(next_permutation(settings.begin(), settings.end()));
  cout << "Part 2: " << ans2 << endl;
}

