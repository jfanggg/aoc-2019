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

// empty out a certain computer's outgoing messages
void send_messages(vector<Computer>& computers, const int& i, 
    Message& last_nat_message, long& ans1) {

  State& state = computers.at(i).state;
  long& output_ctr = computers.at(i).output_ctr;

  while (output_ctr < state.output.size()) {
    long dest = state.output.at(output_ctr);
    long X = state.output.at(output_ctr + 1);
    long Y = state.output.at(output_ctr + 2);

    if (dest == 255) {
      ans1 = (ans1 == -1) ? Y : ans1;
      last_nat_message = {X, Y};
    } else {
      computers.at(dest).q.push({X, Y});
    }
    output_ctr += 3;
  } 
}

int main() {
  auto memory = parse_input();
  vector<Computer> computers;

  long ans1 = -1, ans2 = -1;
  Message last_nat_message;
  unordered_set<long> seen_ys;

  for (int i = 0; i < 50; i++) {
    computers.push_back({{}, run({memory}, {i}), 0});
    send_messages(computers, i, last_nat_message, ans1);
  }

  while (ans1 == -1 || ans2 == -1) {
    bool idle = true;
    for (int i = 0; i < 50; i++) {
      auto& state = computers.at(i).state;
      auto& q = computers.at(i).q;

      idle &= q.empty();
      if (q.empty()) {
        state = run(state, {-1});
      }

      while (!q.empty()) {
        Message m = q.front();
        q.pop();
        state = run(state, {m.first, m.second});
      }
      send_messages(computers, i, last_nat_message, ans1);
    }

    if (idle) {
      int Y = last_nat_message.second;
      ans2 = (seen_ys.find(Y) != seen_ys.end()) ? Y : ans2;
      seen_ys.insert(Y);
      
      computers.at(0).q.push(last_nat_message);
    }
  }
  cout << "Part 1: " << ans1 << endl;
  cout << "Part 2: " << ans2 << endl;
}
