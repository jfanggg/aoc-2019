#include <iostream>
#include <map>
#include <queue>
#include <set>
#include "intcode.h"
using namespace std;

const int dx[] = { 0, 0, -1, 1 };
const int dy[] = { 1, -1, 0, 0 };
typedef pair<int, int> Coordinate;

void bfs(State start, map<Coordinate, int>& dists, map<Coordinate, State>& states, 
    Coordinate& tank, State& tank_state) {

  Coordinate origin = {0, 0};
  queue<Coordinate> q;

  dists[origin] = 0;
  states[origin] = start;
  q.push(origin);

  while (!q.empty()) {
    Coordinate c = q.front();
    q.pop();

    for (int dir = 0; dir < 4; dir++) {
      Coordinate new_c = { c.first + dx[dir], c.second + dy[dir] };
      if (dists.find(new_c) != dists.end()) 
        continue;
      dists[new_c] = dists[c] + 1;

      State new_state = run(states.at(c), {dir + 1});
      int output = new_state.output.at(0);
      new_state.output.clear();

      if (output == 2) {
        tank = new_c;
        tank_state = new_state;
      }
      if (output > 0) {
        states[new_c] = new_state;
        q.push(new_c);
      }
    }
  }
}

int main() {
  auto memory = parse_input();

  map<Coordinate, int> dists;
  map<Coordinate, State> states;

  Coordinate tank, trash_coordinate;
  State tank_state, trash_state;

  bfs(State(memory), dists, states, tank, tank_state);
  cout << "Part 1: " << dists.at(tank) << endl;

  dists.clear();
  states.clear();
  bfs(tank_state, dists, states, trash_coordinate, trash_state);

  int ans2 = 0;
  for (auto iter : dists) {
    if (states.find(iter.first) != states.end()) {
      ans2 = max(ans2, iter.second);
    } 
  }
  cout << "Part 2: " << ans2 << endl;
}
