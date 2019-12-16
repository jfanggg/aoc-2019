#include <iostream>
#include <map>
#include "intcode.h"
using namespace std;
typedef pair<int, int> Coordinate;

int main() {
  auto memory = parse_input();
  State state = State(memory);
  state = run(state);

  int ans1 = 0;
  map<Coordinate, int> picture;
  for (int i = 0; i < state.output.size(); i += 3) {
    int x = state.output.at(i);
    int y = state.output.at(i + 1);
    int id = state.output.at(i + 2);

    if (id == 2) 
      ans1++;
  
    Coordinate coordinate = {x, y};
    picture[coordinate] = id;
  }
  cout << "Part 1: " << ans1 << endl;
}

