#include <iostream>
#include <cmath>
using namespace std;

int main() {
  int x;
  int ans = 0;
  while (cin >> x) {
    ans += floor(x / 3) - 2;
  }
  cout << ans << endl;
}

