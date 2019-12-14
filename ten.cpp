#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
#include <set>
#include <unordered_set>
using namespace std;

#define PI 3.14159265

int gcd(int a, int b) {
  if (a == 0) {
    return b;
  }
  return gcd(b % a, a);
}

int count_visible(vector<string>& input, int r, int c, int R, int C) {
  set<pair<int, int>> seen;
  for (int x = 0; x < R; x++) {
    for (int y = 0; y < C; y++) {
      if (x == r && y == c) 
        continue;

      if (input[x][y] == '#') {
        int dx = x - r;
        int dy = y - c;
        int g = gcd(abs(dx), abs(dy));
        seen.insert({dx / g, dy / g});
      }
    }
  }
  return seen.size();
}

int l1_dist(int x1, int y1, int x2, int y2) {
  return abs(x1 - x2) + abs(y1 - y2);
}

int vaporize(vector<string> input, int r, int c, int R, int C) {
  map<double, vector<pair<int, int>>> asteroid_map;
  for (int x = 0; x < R; x++) {
    for (int y = 0; y < C; y++) {
      if (x == r && y == c) 
        continue;

      if (input[x][y] == '#') {
        int dx = r - x;
        int dy = y - c;
        int g = gcd(abs(dx), abs(dy));
        double angle = atan2(dy / g, dx / g) * 180.0 / PI;
        if (angle < 0)
          angle += 360;

        if (asteroid_map.find(angle) == asteroid_map.end()) {
          asteroid_map[angle] = vector<pair<int, int>>();
        }
        asteroid_map[angle].push_back({x, y});
      }
    }
  }

  int found = 0;
  int ans2 = -1;
  while (ans2 == -1) {
    for (auto angle_itr = asteroid_map.begin(); angle_itr != asteroid_map.end(); angle_itr++) {
      int closest_idx = -1, closest_dist = 1E8;

      // linear scan for closest asteroid for this angle
      auto& asteroids = angle_itr->second;
      for (int i = 0; i < asteroids.size(); i++) {
        int l1 = l1_dist(r, c, asteroids[i].first, asteroids[i].second);
        if (l1 < closest_dist) {
          closest_dist = l1;
          closest_idx = i;
        }
      }
      
      // count asteroid, then vaporize
      if (closest_idx != -1) {
        if (++found == 200) {
          ans2 = asteroids[closest_idx].first + 100 * asteroids[closest_idx].second;
          break;
        }
        asteroids.erase(asteroids.begin() + closest_idx);
      }
    }
  }
  return ans2;
}

int main() {
  string s;
  vector<string> input;
  while (cin >> s) {
    input.push_back(s);
  }

  int R = input.size(), C = input[0].length();
  int ans1 = 0, best_r, best_c;
  for (int r = 0; r < R; r++) {
    for (int c = 0; c < C; c++) {
      if (input[r][c] == '#') {
        int count = count_visible(input, r, c, R, C);
        if (count > ans1) {
          ans1 = count;
          best_r = r;
          best_c = c;
        }
      }
    }
  }
  cout << "Part 1: " << ans1 << endl;
  int ans2 = vaporize(input, best_r, best_c, R, C);
  cout << "Part 2: " << ans2 << endl;
}

