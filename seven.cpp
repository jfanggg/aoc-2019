#include <algorithm>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include "intcode.h"
using namespace std;

char ctoi(int x) {
  return x - '0';
}

int build_num(vector<int> parts) {
  string s = "";
  for (int part : parts) {
    s += to_string(part);
  }
  return stoi(s);
}

int main() {
  vector<int> program;
  string s;
  while(std::getline(cin, s, ',')) {
    program.push_back(stoi(s));
  }

  int ans1 = 0;
  string settings = "01234";

  do {
    int x = 0;
    for (int i = 0; i < 5; i++) {
      x = run(State(program), {ctoi(settings[i]), x}).output[0];
    }
    ans1 = max(ans1, x);
  } while(next_permutation(settings.begin(), settings.end()));
  cout << "Part 1: " << ans1 << endl;

  settings = "56789";
  int ans2 = 0;
  do {
    int x = 0;
    State a(program), b(program), c(program), d(program), e(program);

    // pass in phase value first
    a = run(a, {ctoi(settings[0])});
    b = run(b, {ctoi(settings[1])});
    c = run(c, {ctoi(settings[2])});
    d = run(d, {ctoi(settings[3])});
    e = run(e, {ctoi(settings[4])});

    while (!e.terminated) {
      a = run(a, {x});
      x = a.output[a.output.size() - 1];
      b = run(b, {x});
      x = b.output[b.output.size() - 1];
      c = run(c, {x});
      x = c.output[c.output.size() - 1];
      d = run(d, {x});
      x = d.output[d.output.size() - 1];
      e = run(e, {x});
      x = e.output[e.output.size() - 1];
    }
    ans2 = max(ans2, x);
  } while(next_permutation(settings.begin(), settings.end()));
  cout << "Part 2: " << ans2 << endl;
}

