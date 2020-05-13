#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include "intcode.h"

using namespace std;
typedef pair<long, long> Message;

struct Computer {
  queue<Message> q;
  State state;
  long output_ctr;
};

int main() {
  vector<long> inputs;
  auto memory = parse_input();

  vector<Computer> computers;

  for (int i = 0; i < 50; i++) {
    computers.push_back({{}, {memory}, 0});

    State& state = computers.at(i).state;
    state = run(state, {i});
    long& output_ctr = computers.at(i).output_ctr;

    while (output_ctr < state.output.size()) {
      long dest = state.output.at(output_ctr);
      long X = state.output.at(output_ctr + 1);
      long Y = state.output.at(output_ctr + 2);
      computers.at(dest).q.push({X, Y});
      output_ctr += 3;
    }
  }

  long ans1 = -1, ans2 = -1;
  Message last_nat_message;
  unordered_set<long> nat_sent_ys;

  while (ans1 == -1 || ans2 == -1) {
    bool idle = true;
    for (int i = 0; i < 50; i++) {
      auto& state = computers.at(i).state;
      auto& q = computers.at(i).q;
      long& output_ctr = computers.at(i).output_ctr;

      if (q.empty()) {
        state = run(state, {-1});
      } else {
        idle = false;
        while (!q.empty()) {
          Message m = q.front();
          q.pop();
          state = run(state, {m.first, m.second});
        }
      }

      while (output_ctr < state.output.size()) {
        long dest = state.output.at(output_ctr);
        long X = state.output.at(output_ctr + 1);
        long Y = state.output.at(output_ctr + 2);

        if (dest == 255) {
          if (ans1 == -1) {
            ans1 = Y;
          }
          last_nat_message = {X, Y};
        } else {
          computers.at(dest).q.push({X, Y});
        }
        output_ctr += 3;
      } 
    }

    if (idle) {
      int Y = last_nat_message.second;
      if (nat_sent_ys.find(Y) != nat_sent_ys.end()) {
        ans2 = Y;
      }
      nat_sent_ys.insert(Y);
      computers.at(0).q.push(last_nat_message);
    }
  }
  cout << "Part 1: " << ans1 << endl;
  cout << "Part 2: " << ans2 << endl;
}
