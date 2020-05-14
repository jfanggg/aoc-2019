#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <set>
#include "intcode.h"
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>
using namespace std;

// sort items before constructing
struct Node {
  string room;
  vector<string> inventory;

  Node(string room, vector<string> inventory)
      : room(room)
      , inventory(inventory) {}

  bool operator==(const Node& other) const {
    return room == other.room && inventory == other.inventory;
  }
};

struct node_hash_fn
{
  std::size_t operator() (const Node& node) const
  {
    std::size_t seed = 0;
    boost::hash_combine(seed, node.room);
    for (string s : node.inventory) {
      boost::hash_combine(seed, s);
    }
    return seed;
  }
};

string read_output(State& s, bool consume=true) {
  vector<char> chars;
  for (long l : s.output) {
    chars.push_back((char) l);
  }
  if (consume) {
    s.output.clear();
  }
  return { chars.begin(), chars.end() };
}

void parse_output(string output, string& room, vector<string>& dirs, vector<string>& items) {
  vector<string> lines;
  boost::split(lines, output, boost::is_any_of("\n"));

  int step = 0;
  for (string line : lines) {
    if (line.rfind("Door", 0) == 0) {
      step = 1;
    } 
    if (line.rfind("Items", 0) == 0) {
      step = 2;
    }

    if (line.rfind("== ", 0) == 0) {
      room = line;
    }
    if (line.rfind("- ", 0) == 0) {
      string s = line.substr(2);
      switch (step) {
        case 1:
          dirs.push_back(s + "\n");
          break;
        case 2:
          items.push_back(s + "\n");
          break;
        default:
          cout << "Invalid step" << endl;
      }
    }
  }
}

vector<long> convert_string(string s) {
  vector<long> outputs;
  for (char c : s) {
    outputs.push_back((long) c);
  }
  return outputs;
}

void bfs(State start) {
  unordered_set<string> bad_items = {
    "infinite loop\n",
    "escape pod\n",
    "giant electromagnet\n",
    "photons\n",
    "molten lava\n",
  };

  vector<string> trash;
  string room0;
  string output = read_output(start, false);
  parse_output(output, room0, trash, trash);

  Node n0 = { room0, {} };

  unordered_map<Node, State, node_hash_fn> state_map;
  unordered_map<string, vector<string>> doors; 
  state_map[n0] = start;

  queue<Node> q;
  q.push(n0);

  string ans;
  while (!q.empty()) {
    Node curr = q.front();
    q.pop();

    State state = state_map.at(curr);

    string room;
    vector<string> dirs, items;
    string output = read_output(state);
    parse_output(output, room, dirs, items);

    // for the outputs that don't tell you your current state. 
    if (room == "") {
      room = curr.room;
      dirs = doors.at(room);
    } 
    doors[room] = dirs;

    if (output.find("airlock") != std::string::npos) {
      ans = output;
      break;
    }

    if (state.terminated) {
      continue;
    }

    // move
    for (string dir : dirs) {
      State next_state = run(state, convert_string(dir));
      string next_output = read_output(next_state, false);
      string next_room;
      parse_output(next_output, next_room, trash, trash);

      // check if this move is valid
      if (next_output.find("You can't") != std::string::npos) {
        continue;
      }

      Node next = { next_room, curr.inventory };
      if (state_map.find(next) != state_map.end()) {
        continue;
      }

      state_map[next] = next_state;
      q.push(next);
    }
    // pick up
    for (string item : items) {
      if (bad_items.find(item) != bad_items.end()) {
        continue;
      }
      vector<string> new_inventory(curr.inventory);
      new_inventory.push_back(item);
      sort(new_inventory.begin(), new_inventory.end());

      Node next = { room, new_inventory };
      if (state_map.find(next) != state_map.end()) {
        continue;
      }
      State next_state = run(state, convert_string("take " + item));
      state_map[next] = next_state;
      q.push(next);
    }
    // drop
    for (string item : curr.inventory) {
      vector<string> new_inventory(curr.inventory);
      auto iter = find(new_inventory.begin(), new_inventory.end(), item);
      new_inventory.erase(iter);
      sort(new_inventory.begin(), new_inventory.end());

      Node next = { room, new_inventory };
      if (state_map.find(next) != state_map.end()) {
        continue;
      }

      State next_state = run(state, convert_string("drop " + item));
      state_map[next] = next_state;
      q.push(next);
    }
  }
  cout << "Part 1: " << endl;
  cout << ans;
}

int main() {
  auto memory = parse_input();
  State state = run({memory}, {});

  bfs(state);
}
