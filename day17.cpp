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

  string input_string;
  input_string += "A,C,A,C,A,B,C,B,A,B\n";
  input_string += "R,4,L,10,L,10\n";
  input_string += "L,8,L,8,R,10,R,4\n";
  input_string += "L,8,R,12,R,10,R,4\n";
  input_string += "n\n"; 

  vector<long> inputs;
  for (char c : input_string) {
    inputs.push_back(c);
  }
  memory[0] = 2;

  state = State(memory);
  state = run(state, inputs);
  cout << "Part 2: " << state.output.back() << endl;
}
