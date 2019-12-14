#include <iostream>
#include <map>
#include <set>
#include "intcode.h"
using namespace std;

const int dx[] = { 0, 1, 0, -1 };
const int dy[] = { 1, 0, -1, 0 };

int main() {
  map<pair<int, int>, int> picture;
  set<pair<int, int>> colored;
  auto memory = parse_input(); 

  int dir = 0;
  int x = 0, y = 0;

  State state = State(memory);
  while (!state.terminated) {
    pair<int, int> coordinate = {x, y};
    int current_color = picture[coordinate];
    state = run(state, {current_color});

    int size = state.output.size();
    int color = state.output[size - 2];
    int turn = state.output[size - 1];

    picture[coordinate] = color;
    if (color) {
      colored.insert(coordinate);
    }
    dir = dir + (turn == 0 ? -1 : 1);
    dir = (dir + 4) % 4;
    x += dx[dir];
    y += dy[dir];
  }
  cout << "Part 1: " << colored.size() << endl;
}

