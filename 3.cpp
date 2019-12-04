#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
using namespace std;

map<char, int> dx = {
  {'U',  0}, {'D',  0}, 
  {'L', -1}, {'R',  1}
};
map<char, int> dy = {
  {'U',  1}, {'D', -1}, 
  {'L',  0}, {'R',  0}
};

struct Segment {
  int dir;            // 0 if horizontal, 1 if vertical
  int a;              // coordinate along axis parallel to line
  int b1, b2;         // coordinates along other axis
  bool pos;           // true if segment points in a positive direction
  int cum_steps;

  Segment(int s_x, int s_y, int e_x, int e_y, int cum_steps): cum_steps(cum_steps) {
    if (s_x == e_x) {
      dir = 0;
      a = s_x, b1 = s_y, b2 = e_y; 
    } else {
      dir = 1;
      a = s_y, b1 = s_x, b2 = e_x; 
    }
  }
};

vector<Segment> read_line(string line) {
  string s;
  vector<Segment> segments;

  stringstream line_stream(line);
  int x = 0, y = 0, cum_steps = 0;
  while(std::getline(line_stream, s, ',')) {
    char dir = s[0];
    int len = stoi(s.substr(1));

    int new_x = x + len * dx[dir];
    int new_y = y + len * dy[dir];

    segments.push_back(Segment(x, y, new_x, new_y, cum_steps));

    x = new_x; 
    y = new_y; 
    cum_steps += len;
  }
  return segments;
}

// check if x is between [e1, e2]
bool between(int x, int e1, int e2) {
  return min(e1, e2) <= x && x <= max(e1, e2);
}

// given that [a, b] and [c, d] overlap, return min intersection point
int overlap(int a, int b, int c, int d) {
  return max(min(a, b), min(c, d));
}

int main() {
  string line;

  cin >> line;
  vector<Segment> wire1 = read_line(line);
  cin >> line;
  vector<Segment> wire2 = read_line(line);

  int min_dist = 1E9, min_steps = 1E9;
  for (Segment s1 : wire1) {
    for (Segment s2 : wire2) {
      int dist = 0, steps = 0;
      if (s1.dir == s2.dir) {
        // overlapping intersection at (s1.a, b)
        if (s1.a == s2.a && (between(s1.b1, s2.b1, s2.b2) || between(s1.b2, s2.b1, s2.b1))) {
          int b = overlap(s1.b1, s1.b2, s2.b1, s2.b2);
          steps = s1.cum_steps + s2.cum_steps + abs(b - s1.b1) + abs(b - s2.b1);
          dist = abs(s1.a) + abs(b);
        }
      } else {
        // perpendicular intersection at (s1.a, s2.a)
        if (between(s1.a, s2.b1, s2.b2) && between(s2.a, s1.b1, s1.b2)) {
          steps = s1.cum_steps + s2.cum_steps + abs(s2.a - s1.b1) + abs(s1.a - s2.b1);
          dist = abs(s1.a) + abs(s2.a);
        }
      }

      if (dist != 0) {
        min_dist = min(min_dist, dist);
        min_steps = min(min_steps, steps);
      }
    }
  }

  cout << "Part 1: " << min_dist << endl;
  cout << "Part 2: " << min_steps << endl;
}

