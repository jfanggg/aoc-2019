#include <iostream>
#include <vector>
using namespace std;

int pattern[4] = {0, 1, 0, -1};

int delta[4] = {-1, -1, 1, 1};

int ctoi(char c) {
  return c - '0';
}

char itoc(int i) {
  return '0' + i;
}

string fft(string in, int fft_reps, int in_reps) {
  string new_in = "";
  for (int i = 0; i < in_reps; i++) {
    new_in += in;
  }
  in = new_in;

  int in_len = in.length();
  string out = in;

  for (int t = 0; t < fft_reps; t++) {
    // tail_sum[i] represents sum(in[i:])
    vector<int> tail_sum(in_len, 0);
    tail_sum[in_len - 1] = ctoi(in[in_len - 1]);
    for (int i = in_len - 2; i >= 0; i--) {
      tail_sum[i] = ctoi(in[i]) + tail_sum[i + 1]; 
    }

    for (int o = 0; o < in_len; o++) {
      int pattern_reps = o + 1;
      int sum = tail_sum[o];

      int delta_idx = 0;
      for (int i = o + pattern_reps; i < in_len; i += pattern_reps) {
        sum += tail_sum[i] * delta[delta_idx];
        delta_idx = (delta_idx + 1) % 4;
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

  string out = fft(s, 100, 1);
  cout << "Part 1: " << out.substr(0, 8) << endl;

  int idx = stoi(s.substr(0, 7));
  out = fft(s, 100, 1E4);
  cout << "Part 2: " << out.substr(idx, 8) << endl;
}

