#include <iostream>
using namespace std;

int pattern[4] = {0, 1, 0, -1};

int ctoi(char c) {
  return c - '0';
}

char itoc(int i) {
  return '0' + i;
}

string fft(string in, int reps) {
  string out = in;

  for (int t = 0; t < reps; t++) {
    for (int o = 0; o < in.length(); o++) {
      int sum = 0;
      for (int i = 0; i < in.length(); i++) {
        int pattern_idx = ((i + 1) / (o + 1)) % 4;
        sum += ctoi(in[i]) * pattern[pattern_idx];
      }
      sum = abs(sum) % 10;
      out[o] = itoc(sum);
    }
    in = out;
  }
  return out;
}

int main() {
  string s;
  cin >> s; 

  string out = fft(s, 100);
  cout << "Part 1: " << out.substr(0, 8) << endl;
}

