#include <iostream>
#include <cmath>
using namespace std;

int main() {
  int x, y;
  int ans = 0;
  while (cin >> x) {
    y = floor(x / 3) - 2;
    while (y > 0) {
      ans += y;
      y = floor(y / 3) - 2;
    }
  }
  cout << ans << endl;
}

