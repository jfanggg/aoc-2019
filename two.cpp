#include <iostream>
#include <vector>
#include <cstdlib>
#include "intcode.h"
using namespace std;

int main() {
  string delimiter = ",";
  string s;

  vector<int> nums;
  int x;
  size_t start = 0, end = 0;

  cin >> s;
  while ((end = s.find(delimiter, start)) != std::string::npos) {
    x = atoi(s.substr(start, end).c_str());
    nums.push_back(x);
    start = end + 1;
  }
  x = atoi(s.substr(start).c_str());
  nums.push_back(x);

  // Part 1
  nums[1] = 12;
  nums[2] = 2;
  cout << "Part 1: " << run(nums) << endl;

  // Part 2
  for (int n = 0; n < 100; n++) {
    nums[1] = n;

    for (int v = 0; v < 100; v++) {
      nums[2] = v;
      if (run(nums) == 19690720) {
        cout << "Part 2: " << 100*n + v << endl;
      }
    }
  }
}

