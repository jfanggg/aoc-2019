#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;

int gcd(int a, int b) {
  if (a == 0) {
    return b;
  }
  return gcd(b % a, a);
}

int count_visible(vector<string>& map, int r, int c, int R, int C) {
  set<pair<int, int>> seen;
  for (int x = 0; x < R; x++) {
    for (int y = 0; y < C; y++) {
      if (x == r && y == c) 
        continue;

      if (map[x][y] == '#') {
        int dx = x - r;
        int dy = y - c;
        int g = gcd(abs(dx), abs(dy));
        seen.insert({dx / g, dy / g});
      }
    }
  }
  return seen.size();
}

int main() {
  string s;
  vector<string> map;
  while (cin >> s) {
    map.push_back(s);
  }

  int R = map.size(), C = map[0].length();
  int ans1 = 0;
  for (int r = 0; r < R; r++) {
    for (int c = 0; c < C; c++) {
      if (map[r][c] == '#') {
        int count = count_visible(map, r, c, R, C);
        ans1 = max(ans1, count);
      }
    }
  }

  cout << "Part 1: " << ans1 << endl;
}

