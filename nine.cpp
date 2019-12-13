#include <iostream>
#include <vector>
#include "intcode.h"

using namespace std;

int main() {
  auto memory = parse_input();
  State state0(memory);

  for (int i = 1; i <= 2; i++) {
    auto inputs = vector<long>({i});
    State state = run(state0, inputs);
    cout << "Part " << i << ":";
    for (long o : state.output) {
      cout << o << " ";
    }
    cout << endl;
  }
}
