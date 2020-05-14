#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
using namespace std;

const int TOP = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 3;

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

int to_idx(const int& r, const int& c) {
  return 5 * r + c;
}

bool in_bounds(const int& r, const int& c) {
  return 0 <= r && r < 5 && 0 <= c && c < 5;
}

void update_idx(int& new_board, int idx, int bug, int adj_count) {
  if (bug && adj_count != 1) {
    set_bit(new_board, idx, 0);
  } else if (!bug && (adj_count == 1 || adj_count == 2)) {
    set_bit(new_board, idx, 1);
  } else {
    set_bit(new_board, idx, bug);
  }
}

int count_edge(const int& board, int edge) {
  int count = 0;
  switch (edge) {
    case TOP:
      for (int i = 0; i < 5; i++) {
        count += check_bit(board, i);
      }
      break;
    case LEFT:
      for (int i = 0; i < 25; i += 5) {
        count += check_bit(board, i);
      }
      break;
    case RIGHT:
      for (int i = 4; i < 25; i += 5) {
        count += check_bit(board, i);
      }
      break;
    case BOTTOM:
      for (int i = 20; i < 25; i++) {
        count += check_bit(board, i);
      }
      break;
  }
  return count;
}

int count_bugs(int board) {
  int ctr = 0;
  for (int i = 0; i < 25; i++) {
    ctr += (board >> i) & 1;
  }
  return ctr;
}

void visualize(int board) {
  for (int r = 0; r < 5; r++) {
    for (int c = 0; c < 5; c++) {
      cout << (check_bit(board, 5 * r + c) ? '#' : '.');
    }
    cout << endl;
  }
  cout << endl;
}

int solve(int board) {
  unordered_set<int> seen_states;
  seen_states.insert(board);
  while (1) {
    int new_board = 0;
    for (int idx = 0; idx < 25; idx++) {
      int prev_bit = check_bit(board, idx);

      int adj_count = 0;
      adj_count += (idx > 4)     && check_bit(board, idx - 5);
      adj_count += (idx < 20)     && check_bit(board, idx + 5);
      adj_count += (idx % 5 != 0) && check_bit(board, idx - 1);
      adj_count += (idx % 5 != 4) && check_bit(board, idx + 1);

      update_idx(new_board, idx, prev_bit, adj_count);
    }
    board = new_board;
    if (seen_states.find(board) != seen_states.end()) {
      break;
    }
    seen_states.insert(board);
  }
  return board;
}

int solve_recursive(int board) {
  unordered_map<int, int> levels;
  levels[0] = board;

  for (int iter = 0; iter < 200; iter++) {
    unordered_set<int> relevant_levels;
    for (auto kv : levels) {
      relevant_levels.insert(kv.first);
      relevant_levels.insert(kv.first + 1);
      relevant_levels.insert(kv.first - 1);
    }

    unordered_map<int, int> new_levels;
    for (auto level : relevant_levels) {
      int board = 0;
      int o_board = 0;
      int i_board = 0;

      if (levels.find(level) != levels.end()) {
        board = levels.at(level);
      }
      if (levels.find(level - 1) != levels.end()) {
        i_board = levels.at(level - 1);
      }
      if (levels.find(level + 1) != levels.end()) {
        o_board = levels.at(level + 1);
      }

      int new_board = 0;
      for (int idx = 0; idx < 25; idx++) {
        // middle square
        if (idx == 12)
          continue;
        int prev_bit = check_bit(board, idx);

        int adj_count = 0;
        adj_count += (idx >   4)    && check_bit(board, idx - 5);
        adj_count += (idx <  20)    && check_bit(board, idx + 5);
        adj_count += (idx % 5 != 0) && check_bit(board, idx - 1);
        adj_count += (idx % 5 != 4) && check_bit(board, idx + 1);

        // check outer board
        adj_count += (idx <=  4)    && check_bit(o_board, 7);
        adj_count += (idx >= 20)    && check_bit(o_board, 17);
        adj_count += (idx % 5 == 0) && check_bit(o_board, 11);
        adj_count += (idx % 5 == 4) && check_bit(o_board, 13);

        // check inner board
        adj_count += (idx !=  7) ? 0 : count_edge(i_board, TOP);
        adj_count += (idx != 17) ? 0 : count_edge(i_board, BOTTOM);
        adj_count += (idx != 11) ? 0 : count_edge(i_board, LEFT);
        adj_count += (idx != 13) ? 0 : count_edge(i_board, RIGHT);
        update_idx(new_board, idx, prev_bit, adj_count);
      }
      new_levels[level] = new_board;
    }
    levels = new_levels;
  }
  int total_bugs = 0;
  for (auto kv : levels) {
    total_bugs += count_bugs(kv.second);
  }
  return total_bugs;
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
  cout << "Part 1: " << solve(board) << endl;
  cout << "Part 2: " << solve_recursive(board) << endl;
}