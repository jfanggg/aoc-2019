
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

int fuel(vector<int> masses, bool recursive) {
  int ans = 0;
  for (int x : masses) {
    int y = floor(x / 3) - 2;

    if (recursive) {
      while (y > 0) {
        ans += y;
        y = floor(y / 3) - 2;
      }
    } else {
      ans += y;
    }
  }
  return ans;
}

int main() {
  int x;
  vector<int> masses;
  while (cin >> x) {
    masses.push_back(x);
  }

  cout << "Part 1: " << fuel(masses, false) << endl;;
  cout << "Part 2: " << fuel(masses, true) << endl;;
}