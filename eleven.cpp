#include <iostream>
#include <map>
#include <set>
#include "intcode.h"
using namespace std;

const int dx[] = { 0, 1, 0, -1 };
const int dy[] = { 1, 0, -1, 0 };
typedef pair<int, int> Coordinate;

void draw(unordered_map<long, long> memory, int start_color, 
    map<Coordinate, int>& picture, set<Coordinate>& colored) {

  int dir = 0;
  int x = 0, y = 0;
  Coordinate coordinate = {x, y};
  picture[coordinate] = start_color;
  
  State state = State(memory);
  while (!state.terminated) {
    coordinate = {x, y};
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
}

int main() {
  auto memory = parse_input(); 
  map<Coordinate, int> picture;
  set<Coordinate> colored;

  draw(memory, 0, picture, colored);
  cout << "Part 1: " << colored.size() << endl;

  picture.clear();
  colored.clear();
  draw(memory, 1, picture, colored);
  int min_x = INT_MAX, min_y = INT_MAX, max_x = INT_MIN, max_y = INT_MIN;
  for (auto iter : picture) {
    int x = iter.first.first;
    int y = iter.first.second;

    min_x = min(x, min_x);
    max_x = max(x, max_x);
    min_y = min(y, min_y);
    max_y = max(y, max_y);
  }
  cout << "Part 2: " << endl;
  for (int y = max_y; y >= min_y; y--) {
    for (int x = min_x; x <= max_x; x++) {
      Coordinate coordinate = {x, y};
      cout << (picture[coordinate] ? "#" : " ");
    }
    cout << endl;
  }
}
