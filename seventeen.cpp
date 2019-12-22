#include <iostream>
#include <algorithm>
#include "intcode.h"
using namespace std;

int dx [5] = { 0, 1, 0, -1, 0 };
int dy [5] = { 0, 0, 1, 0, -1 };

void print_map(vector<vector<char>>& map) {
  for (int r = 0; r < map.size(); r++) {
    for (int c = 0; c < map.at(0).size(); c++) {
      cout << map.at(r).at(c) << " ";
    }
    cout << endl;
  }
}

int main() {
  auto memory = parse_input(); 
  State state = State(memory);
  state = run(state);

  vector<vector<char>> map;
  vector<char> row;
  for (int i = 0; i < state.output.size(); i++) {
    if (state.output.at(i) != 10) {
      row.push_back(state.output.at(i)); 
    } else if (row.size() > 0) {
      map.push_back(row);
      row.clear();
    }
  }

  int ans1 = 0;
  for (int r = 1; r < map.size() - 1; r++) {
    for (int c = 1; c < map.at(0).size() - 1; c++) {
      bool intersection = true;
      for (int d = 0; d < 5; d++) {
        intersection &= map.at(r + dx[d]).at(c + dy[d]) != '.';
      }
      if (intersection) {
        ans1 += r * c;
      }
    }
  }
  cout << "Part 1: " << ans1 << endl;

  long A = 'A', B = 'B', C = 'C', L = 'L', R = 'R', COM = ',', NL = '\n';
  vector<long> inputs = {
    A, COM, C, COM, A, COM, C, COM, A, COM, B, COM, C, COM, B, COM, A, COM, B, NL,
    R, COM, '4', COM, L, COM, '1', '0', COM, L, COM, '1', '0', NL,
    L, COM, '8', COM, L, COM, '8', COM, R, COM, '1', '0', COM, R, COM, '4', NL, 
    L, COM, '8', COM, R, COM, '1', '2', COM, R, COM, '1', '0', COM, R, COM, '4', NL,
    'n', NL
  }; 
  memory[0] = 2;

  state = State(memory);
  state = run(state, inputs);
  cout << "Part 2: " << state.output.back() << endl;
}
