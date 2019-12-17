#include <iostream>
#include <map>
#include <queue>
#include <set>
#include "intcode.h"
using namespace std;

const int dx[] = { 0, 0, -1, 1 };
const int dy[] = { 1, -1, 0, 0 };
typedef pair<int, int> Coordinate;

int main() {
  auto memory = parse_input();

  map<Coordinate, int> dists;
  map<Coordinate, State> states;
  queue<Coordinate> q;

  Coordinate origin = {0, 0};
  dists[origin] = 0;
  states[origin] = State(memory);
  q.push(origin);

  int ans1 = -1;
  while (ans1 == -1) {
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

      if (output == 1) {
        states[new_c] = new_state;
        q.push(new_c);
      } else if (output == 2) {
        ans1 = dists[new_c];
        break;
      }
    }
  }
  cout << "Part 1: " << ans1 << endl;
}
