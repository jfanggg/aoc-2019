#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;

typedef pair<int, int> Command;
const int CUT = 0;
const int REVERSE = 1;
const int DEAL_INCREMENT = 2;

int128_t mod(int128_t x, int128_t m) {
  while (x < 0)
    x += m;
  return x % m;
}

int128_t do_command(Command command, int128_t deck_size, int128_t x) {
  int128_t n = command.second;
  switch (command.first) {
    case CUT:
      n = mod(n, deck_size);
      return mod(x - n, deck_size);
    case REVERSE:
      return deck_size - 1 - x;
    case DEAL_INCREMENT:
      return mod(x * n, deck_size);
    default:
      return -1;
  }
}

// finds x, y s.t. ax + by = gcd(a, b)
void extended_euclid(int128_t a, int128_t b, int128_t& x, int128_t& y) {
  if(a % b == 0){
    x = 0;
    y = 1;
    return;
  }
  extended_euclid(b, a % b, x, y);
  int128_t temp = x;
  x = y;
  y = temp - y * (a / b);
}

// finds x s.t a*x = 1 (mod m)
int128_t mod_inv(int128_t a, int128_t m) {
  int128_t x, y;
  extended_euclid(a, m, x, y);
  return mod(x, m);
}

// solve for a, b s.t. ax + b = shuffle^{-1}(x) (mod m)
pair<int128_t, int128_t> inverse_coeffs(const vector<Command>& commands, 
    const int128_t& deck_size) {

  int128_t a = 1, b = 0;  
  for (int i = commands.size() - 1; i >= 0; i--) {
    auto command = commands.at(i);
    int128_t n = command.second;
    
    switch (command.first) {
      case CUT:
        b += n;
        break;
      case REVERSE:
        b -= (deck_size - 1);
        a = -a;
        b = -b;
        break;
      case DEAL_INCREMENT:
        int128_t inv = mod_inv(n, deck_size);
        a *= inv;
        b *= inv;
        break;
    }
    // reduce a and b
    a = mod(a, deck_size);
    b = mod(b, deck_size);
  }
  return {a, b};
}

int main() {
  string s;
  vector<string> lines;
  vector<Command> commands;
  while (getline(cin, s)) {
    lines.push_back(s);
  }
  for (string line : lines) {
    vector<string> words;
    boost::split(words, line, boost::is_any_of(" "));
    string first = words.front();
    string last = words.back();

    if (first == "cut") {
      commands.push_back({CUT, stoi(last)});
    } else if (first == "deal" && last == "stack") {
      commands.push_back({REVERSE, 0});
    } else if (first == "deal") {
      commands.push_back({DEAL_INCREMENT, stoi(last)});
    }
  }

  int128_t deck_size = 10007;
  int128_t x = 2019;
  for (auto command : commands) {
    x = do_command(command, deck_size, x);
  }
  cout << "Part 1: " << x << endl;

  deck_size = 119315717514047;
  int128_t shuffles = 101741582076661;
  int128_t y = 2020;

  auto coeffs = inverse_coeffs(commands, deck_size);
  int128_t a = coeffs.first;
  int128_t b = coeffs.second;

  // a_exps[i] = a^{2^i}
  vector<int128_t> a_exps;
  a_exps.push_back(a);
  for (int i = 1; i < log2((long) shuffles); i++) {
    int128_t prev = a_exps.at(i - 1);
    int128_t curr = mod(prev * prev, deck_size);
    a_exps.push_back(curr);
  }

  // accumulate a^{shuffles}
  int128_t accumulator = 1;
  int128_t remain_shuffles = shuffles;
  for (int exp_ctr = a_exps.size() - 1; exp_ctr >= 0; exp_ctr--) {
    long num_shuffles = pow(2, exp_ctr);
    if (num_shuffles <= remain_shuffles) {
      remain_shuffles -= num_shuffles;
      accumulator = mod(accumulator * a_exps.at(exp_ctr), deck_size);
    }
  }

  // frac = 1 + a + ... + a^{shuffle - 1}
  //      = (a^{shuffle} - 1) / (a - 1)
  int128_t numer = mod(accumulator - 1, deck_size);
  int128_t denom = mod(a - 1, deck_size);
  int128_t frac = mod(numer * mod_inv(denom, deck_size), deck_size);

  // index = a^{shuffles} * y + (1 + a + a^2 + ... + a^{shuffles-1}) * b
  int128_t index = mod(y * accumulator, deck_size);
  index = mod(index + mod(b * frac, deck_size), deck_size);

  cout << "Part 2: " << index << endl;
}