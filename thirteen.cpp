#include <iostream>
#include <map>
#include "intcode.h"
using namespace std;
typedef pair<int, int> Coordinate;

const int BLOCK = 2;
const int PADDLE = 3;
const int BALL = 4;

void draw_output(State& state, map<Coordinate, int>& picture, int& score) {
  bool cleared = true;

  for (int i = 0; i < state.output.size(); i += 3) {
    int x = state.output.at(i);
    int y = state.output.at(i + 1);
    int id = state.output.at(i + 2);

    if (x == -1 && y == 0) {
      score = id;
      continue;
    }

    Coordinate coordinate = {x, y};
    picture[coordinate] = id;
  }
  score = cleared ? score : -1;
}

void find(map<Coordinate, int>& picture, Coordinate& ball, Coordinate& paddle) {
  for (auto iter : picture) {
    if (iter.second == BALL) {
      ball = iter.first;
    } else if (iter.second == PADDLE) {
      paddle = iter.first;
    }
  }
}

bool won(map<Coordinate, int>& picture) {
  for (auto iter : picture) {
    if (iter.second == BLOCK) {
      return false;
    } 
  }
  return true;
}

void print_output(map<Coordinate, int>& picture) {
  for (int x = 0; x < 38; x++) {
    for (int y = 0; y < 20; y++) {
      Coordinate c = {x, y};
      if (picture[c] == 0) {
        cout << " ";
      } else {
        cout << picture[c];
      }
    }
    cout << endl;
  }
}

int main() {
  auto memory = parse_input();
  State state = State(memory);
  state = run(state);
  int score = -1;
  Coordinate ball, paddle;

  map<Coordinate, int> picture;
  draw_output(state, picture, score);
  int ans1 = 0;
  for (auto iter : picture) {
    if (iter.second == BLOCK) {
      ans1++;
    }
  }
  cout << "Part 1: " << ans1 << endl;

  memory[0] = 2;
  state = State(memory);
  while (1) {
    int dir = ball.first - paddle.first;
    dir = (dir == 0) ? dir : dir/abs(dir);

    state.output.clear();
    state = run(state, {dir});
    draw_output(state, picture, score);
    if (won(picture)) 
      break;

    find(picture, ball, paddle);
  }
  cout << "Part 2: " << score << endl;
}
