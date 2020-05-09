#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
using namespace std;

typedef pair<int, int> Coordinate;
const int dr[4] = {1, 0, -1, 0};
const int dc[4] = {0, 1, 0, -1};
const int MAX_BOARD_SIZE = 100;

int dists[MAX_BOARD_SIZE][MAX_BOARD_SIZE];  // used for BFS

struct Input {
  vector<string> maze;
  unordered_map<char, Coordinate> keys;
  unordered_map<char, Coordinate> doors;

  Input(vector<string> maze, unordered_map<char, Coordinate> keys, 
    unordered_map<char, Coordinate> doors)
      : maze(maze)
      , keys(keys)
      , doors(doors) 
  {}
};

void evaluate(const Input& input, const Coordinate& loc, 
    const unordered_set<char>& remain_keys, 
    unordered_map<char, int>& visible_key_dists, int& heuristic);

struct State {
  int cost;
  int total_heuristic;  // cost + heuristic for remainder
  string hash;
  Coordinate location;
  unordered_set<char> remain_keys;
  unordered_map<char, int> visible_key_dists; 

  State(const Input& input, int cost, string hash,
    Coordinate location, unordered_set<char> remain_keys)
      : cost(cost)
      , hash(hash)
      , location(location)
      , remain_keys(remain_keys) {
    
    int h;
    evaluate(input, location, remain_keys, visible_key_dists, h);
    total_heuristic = cost + h;
  }
};

bool is_key(char c) {
  return 'a' <= c && c <= 'z';
}
bool is_door(char c) {
  return 'A' <= c && c <= 'Z';
}
char lower(char c) {
  return c - 'A' + 'a';
} 

string hash_state(const Coordinate& coord, const unordered_set<char>& remain_keys) {
  string hash = to_string(coord.first) + "," + to_string(coord.second) + ":";
  for (char c = 'a'; c <= 'z'; c++) {
    if (remain_keys.find(c) != remain_keys.end()) {
      hash += c;
    }
  }
  return hash;
}

int hash_coord(Coordinate c) {
  return MAX_BOARD_SIZE * c.first + c.second;
}

// BFS to find visible keys and heuristic using remaining keys
void evaluate(const Input& input, const Coordinate& loc, 
    const unordered_set<char>& remain_keys, 
    unordered_map<char, int>& visible_key_dists, int& heuristic) {

  memset(dists, -1, 100 * 100 * sizeof(dists[0][0]));

  // these use an int from `hash_coord` instead of using Coordinate directly
  unordered_set<int> blocked;
  unordered_map<int, int> parent;
  unordered_map<char, int> remain_keys_coords;

  queue<Coordinate> q;
  dists[loc.first][loc.second] = 0;
  q.push(loc);
  while (!q.empty()) {
    Coordinate coord = q.front();
    q.pop();

    int r = coord.first;
    int c = coord.second;
    int hash = hash_coord(coord);
    bool is_blocked = blocked.find(hash) != blocked.end();

    for (int i = 0; i < 4; i++) {
      int new_r = r + dr[i];
      int new_c = c + dc[i];
      int new_hash = hash_coord({new_r, new_c});
      char letter = input.maze.at(new_r).at(new_c);

      // skip if it's a wall or we've been here before
      if (letter == '#')
        continue;
      if (dists[new_r][new_c] != -1) 
        continue;
      dists[new_r][new_c] = dists[r][c] + 1;

      // we're in a blocked part if inherited or at a door
      is_blocked |= is_door(letter) && remain_keys.find(lower(letter)) != remain_keys.end();
      if (is_blocked) {
        blocked.insert(new_hash);
      }

      // found a key. Record if it's a remaining or visible
      if (is_key(letter)) {
        if (remain_keys.find(letter) != remain_keys.end()) {
          if (!is_blocked) {
            visible_key_dists[letter] = dists[new_r][new_c];
          }
          remain_keys_coords[letter] = new_hash;
        }
      }
      parent[new_hash] = hash;
      q.push({new_r, new_c});
    }
  }

  // compute heuristic using remain_keys_coords
  if (remain_keys_coords.empty()) {
    heuristic = 0;
  } else {
    unordered_set<int> seen;
    for (auto kv : remain_keys_coords) {
      int hash = kv.second;
      seen.insert(hash);
      while (parent.find(hash) != parent.end()) {
        hash = parent.at(hash);
        seen.insert(hash);
      }
    }
    // number of nodes to visit to get to all keys, excluding start
    heuristic = seen.size() - 1;
  }
}

int a_star(const Input& input, const Coordinate& start) {
  unordered_set<char> remain_keys;
  for (auto kv : input.keys) {
    remain_keys.insert(kv.first);
  }
  string hash = hash_state(start, remain_keys);
  State s0 = {input, 0, hash, start, remain_keys};

  // seen states -> smallest cost to get there
  unordered_map<string, int> seen;

  auto comp = [](State a, State b) { return a.total_heuristic > b.total_heuristic; };
  priority_queue<State, vector<State>, decltype(comp)> q(comp);
  q.push(s0);

  while(!q.empty()) {
    State current = q.top();
    q.pop();

    // found the answer
    if (current.remain_keys.empty())
     return current.cost;

    if (seen.find(current.hash) != seen.end() && current.cost > seen.at(current.hash))
      continue;
    seen[current.hash] = current.cost;

    for (auto kv : current.visible_key_dists) {
      char key = kv.first;
      int cost = kv.second;

      auto n_cost = current.cost + cost;
      auto n_location = input.keys.at(key);
      auto n_keys = current.remain_keys;
      n_keys.erase(key);

      string state_hash = hash_state(n_location, n_keys);
      if (seen.find(state_hash) != seen.end() && n_cost >= seen.at(state_hash))
        continue;
      seen[state_hash] = n_cost;

      State next = {input, n_cost, state_hash, n_location, n_keys};
      q.push(next);
    }
  }
  return -1;
}

int main() {
  string s;
  vector<string> maze;
  
  while (cin >> s) {
    maze.push_back(s);
  }
  Coordinate start = {-1, -1};
  unordered_map<char, Coordinate> keys;
  unordered_map<char, Coordinate> doors;

  for (int r = 0; r < maze.size(); r++) {
    for (int c = 0; c < maze.at(0).size(); c++) {
      char letter = maze.at(r).at(c);
      if (letter == '@') {
        start = {r, c};
        maze[r][c] = '.';
      } else if (is_key(letter)) {
        keys[letter] = {r, c};
      } else if (is_door(letter)) {
        doors[lower(letter)] = {r, c};
      }
    }
  }
  Input input = {maze, keys, doors};

  int path_len = a_star(input, start);
  cout << "Part 1: " << path_len << endl;
}