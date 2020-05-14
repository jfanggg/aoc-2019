#include <cmath>
#include <iostream>
using namespace std;

int to_int(int digits[6]) {
  int val = 0;
  for (int i = 0; i < 5; i++) {
    val = (val + digits[i]) * 10;
  }
  val += digits[5];
  return val;
}

void increment(int digits[6]) {
  for (int i = 5; i >= 0; i--) {
    digits[i] = (digits[i] + 1) % 10;
    if (digits[i] != 0) {
      break;
    }
  }
}

// make digits non-decreasing
void make_non_dec(int digits[6]) {
  int overwrite = -1;
  for (int i = 1; i < 6; i++) {
    if (overwrite != -1) {
      digits[i] = overwrite;
    }
    else if (digits[i] < digits[i - 1]) {
      overwrite = digits[i - 1];
      digits[i] = overwrite;
    }
  }
}

// make digits have a streak of at least 2
void make_geq_2adj(int digits[6]) {
  bool adj = false;
  for (int i = 1; i < 6; i++) {
    if (digits[i] == digits[i - 1]) {
      adj = true;
    }
  }

  if (!adj) {
    digits[4] = digits[5];
  }
}

// make digits have a streak of exactly 2
void make_2adj(int digits[6]) {
  while(1) {
    int has_2 = false;
    int streak_val = digits[0], streak_len = 1;
    for (int i = 1; i < 6; i++) {
      if (digits[i] == streak_val) {
        streak_len++;
      } else {
        has_2 |= (streak_len == 2);
        streak_val = digits[i], streak_len = 1;
      }
    }
    has_2 |= (streak_len == 2);

    if (has_2) {
      break;
    }

    increment(digits);
    make_non_dec(digits);
  }
}

int make_valid(int digits[6], int problem) {
  make_non_dec(digits);

  if (problem == 1) 
    make_geq_2adj(digits);
  if (problem == 2) 
    make_2adj(digits);

  return to_int(digits);
}

int solve(int problem) {
  int counter = 0;
  int digits[] = {3, 5, 3, 0, 9, 6};
  int val = make_valid(digits, problem);

  while (val <= 843212) {
    counter++;

    increment(digits);
    val = make_valid(digits, problem);
  }
  return counter;
}

int main() {
  cout << "Part 1: " << solve(1) << endl;
  cout << "Part 2: " << solve(2) << endl;
}
