#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

int run(vector<int> nums) {
  int idx = 0;
  while (1) {
    int opcode = nums[idx];
    if (opcode == 99) {
      break;
    }

    int a = nums[nums[idx + 1]];
    int b = nums[nums[idx + 2]];
    nums[nums[idx + 3]] = (opcode == 1) ? a + b : a * b;

    idx += 4;
  }
  return nums[0];
}

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
  nums[1] = 12;
  nums[2] = 2;

  // Part 1
  cout << run(nums) << endl;

  // Part 2
  for (int n = 0; n < 100; n++) {
    nums[1] = n;

    for (int v = 0; v < 100; v++) {
      nums[2] = v;
      if (run(nums) == 19690720) {
        cout << 100*n + v << endl;
      }
    }
  }
}

