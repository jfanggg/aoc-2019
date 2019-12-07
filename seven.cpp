#include <algorithm>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include "intcode.h"
using namespace std;

int run_amp(vector<int> program, int phase, int input) {
  vector<int> inputs = vector<int>({phase, input});
  vector<int> outputs;

  outputs = run(program, inputs);
  return outputs[0];
}

int main() {
  vector<int> nums, program;
  string s;
  while(std::getline(cin, s, ',')) {
    nums.push_back(stoi(s));
  }

  int ans1 = 0;
  string settings = "01234";

  do {
    int x = 0;
    x = run_amp(nums, settings[0] - '0', x);
    x = run_amp(nums, settings[1] - '0', x);
    x = run_amp(nums, settings[2] - '0', x);
    x = run_amp(nums, settings[3] - '0', x);
    x = run_amp(nums, settings[4] - '0', x);

    ans1 = max(ans1, x);
  } while(next_permutation(settings.begin(), settings.end()));

  cout << "Part 1: " << ans1 << endl;
}

