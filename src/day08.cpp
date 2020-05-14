#include <iostream>
#include <string>
using namespace std;

int main() {
  int w = 25, h = 6;
  string s;
  cin >> s;

  int least_zeros = w * h + 1, ans1 = 0;
  string image(w * h, '*');

  for (int i = 0; i < s.length(); i += w * h) {
    string layer = s.substr(i, w * h);

    int nums[3] = {0, 0, 0};
    for (int j = 0; j < w * h; j++) {
      // Part 1
      int x = layer[j] - '0';
      if (x <= 2) {
        nums[x]++;
      }

      // Part 2
      if (x <= 1 && image[j] == '*') {
        image[j] = x == 0 ? ' ' : '#';
      }
    }
    if (nums[0] < least_zeros) {
      least_zeros = nums[0];
      ans1 = nums[1] * nums[2];
    }
  }
  cout << "Part 1: " << ans1 << endl;

  cout << "Part 2: " << endl;
  for (int row = 0; row < h; row++) {
    cout << image.substr(row * w, w) << endl;
  }
}

