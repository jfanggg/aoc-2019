#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

const int dr[4] = {1, 0, -1, 0};
const int dc[4] = {0, 1, 0, -1};

void set_bit(int& board, const int& idx, const int& val) {
  if (val == 0) {
    board &= ~(1 << idx);
  } else {
    board |= (1 << idx);
  }
}

int check_bit(const int& board, const int& idx) {
  return (board >> idx) & 1;
}

int coord_to_idx(const int& r, const int& c) {
  return 5 * r + c;
}

bool in_bounds(const int& r, const int& c) {
  return 0 <= r && r < 5 && 0 <= c && c < 5;
}

void visualize(const int& board) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      int idx = coord_to_idx(i, j);
      cout << (check_bit(board, idx) ? '#' : '.');
    }
    cout << endl;
  }
  cout << endl;
}

int main() {
  string s;
  int board = 0;

  int ctr = 0;
  while (cin >> s) {
    for (char c : s) {
      if (c == '#') {
        set_bit(board, ctr, 1);
      }
      ctr++;
    }
  }

  unordered_set<int> seen_states;
  seen_states.insert(board);
  while (1) {
    int new_board = 0;
    for (int r = 0; r < 5; r++) {
      for (int c = 0; c < 5; c++) {
        int idx = coord_to_idx(r, c);
        int bug = check_bit(board, idx);

        int adj_count = 0;
        for (int i = 0; i < 4; i++) {
          int adj_idx = coord_to_idx(r + dr[i], c + dc[i]);

          if (!in_bounds(r + dr[i], c + dc[i]))
            continue;
          if (check_bit(board, adj_idx)) {
            adj_count++;
          }
        }

        if (bug && adj_count != 1) {
          set_bit(new_board, idx, 0);
        } else if (!bug && (adj_count == 1 || adj_count == 2)) {
          set_bit(new_board, idx, 1);
        } else {
          set_bit(new_board, idx, bug);
        }
      }
    }
    board = new_board;
    if (seen_states.find(board) != seen_states.end()) {
      break;
    }
    seen_states.insert(board);
  }
  cout << "Part 1: " << board << endl;
}