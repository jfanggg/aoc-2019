#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
using namespace std;

typedef pair<int, int> Coordinate;
const int dr[4] = {1, 0, -1, 0};
const int dc[4] = {0, 1, 0, -1};

int dists[100][100];  // used for BFS

struct Input {
  int R, C;
  vector<string> maze;
  unordered_map<char, Coordinate> keys;
  unordered_map<char, Coordinate> doors;

  Input(int R, int C, vector<string> maze, unordered_map<char, Coordinate> keys, 
    unordered_map<char, Coordinate> doors)
      : R(R)
      , C(C)
      , maze(maze)
      , keys(keys)
      , doors(doors) 
  {}
};

string hash_state(const Coordinate& coord, const unordered_set<char>& remain_keys) {
  string hash = to_string(coord.first) + "," + to_string(coord.second) + ":";
  for (char c = 'a'; c <= 'z'; c++) {
    if (remain_keys.find(c) != remain_keys.end()) {
      hash += c;
    }
  }
  return hash;
}

struct State {
  int cost;
  int total_heuristic;  // cost + heuristic for remainder
  string hash;
  Coordinate location;
  unordered_set<char> remain_keys;
  unordered_set<char> remain_doors;
  unordered_map<char, int> visible_key_dists; 

  State(int cost, int total_heuristic, Coordinate location,
    unordered_set<char> remain_keys, unordered_set<char> remain_doors,
    unordered_map<char, int> visible_key_dists)
      : cost(cost)
      , total_heuristic(total_heuristic)
      , location(location)
      , remain_keys(remain_keys)
      , remain_doors(remain_doors)
      , visible_key_dists(visible_key_dists) {
    
    hash = hash_state(location, remain_keys);
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

int hash_coord(Coordinate c) {
  return 100 * c.first + c.second;
}

// BFS to find visible keys and heuristic using remaining keys
void evaluate(const Input& input, const Coordinate& loc, 
    const unordered_set<char>& remain_keys, 
    const unordered_set<char>& remain_doors,
    unordered_map<char, int>& visible_key_dists, int& heuristic) {

  unordered_map<char, int> remain_keys_coords;

  memset(dists, -1, 100 * 100 * sizeof(dists[0][0]));
  queue<Coordinate> q;
  unordered_set<int> blocked;
  unordered_map<int, int> parent;
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

      // set distance
      dists[new_r][new_c] = dists[r][c] + 1;

      // we're in a blocked part if inherited or at a door
      if (is_door(letter) && remain_doors.find(lower(letter)) != remain_doors.end()) {
        is_blocked = true;
      }
      if (is_blocked) {
        blocked.insert(new_hash);
      }

      // found a key. Record if it's a remaining key
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
  unordered_set<char> remain_doors;
  for (auto kv : input.keys) {
    remain_keys.insert(kv.first);
  }
  for (auto kv : input.doors) {
    remain_doors.insert(kv.first);
  }

  int heuristic;
  unordered_map<char, int> visible_key_dists; 
  evaluate(input, start, remain_keys, remain_doors, visible_key_dists, heuristic);

  State s0 = {0, heuristic, start, remain_keys, remain_doors, visible_key_dists};

  auto comp = [](State a, State b) { return a.total_heuristic > b.total_heuristic; };
  unordered_map<string, int> seen;
  priority_queue<State, vector<State>, decltype(comp)> q(comp);
  q.push(s0);

  while(!q.empty()) {
    State current = q.top();
    q.pop();

    // found the answer
    if (current.remain_keys.empty())
     return current.cost;

    if (seen.find(current.hash) != seen.end() && current.cost > seen.at(current.hash)) {
      continue;
    }
    seen[current.hash] = current.cost;

    for (auto kv : current.visible_key_dists) {
      char key = kv.first;
      int cost = kv.second;

      auto n_cost = current.cost + cost;
      auto n_location = input.keys.at(key);
      auto n_keys = current.remain_keys;
      auto n_doors = current.remain_doors;
      n_keys.erase(key);
      n_doors.erase(key);

      string state_hash = hash_state(n_location, n_keys);
      if (seen.find(state_hash) != seen.end() && n_cost >= seen.at(state_hash)) {
        continue;
      }
      seen[state_hash] = n_cost;

      int h;
      unordered_map<char, int> n_visible_key_dists; 
      evaluate(input, n_location, n_keys, n_doors, n_visible_key_dists, h);

      /*
      cout << "A*: loc = (" << n_location.first << ", " << n_location.second << ")" << endl;
      cout << "cost: " << n_cost << ", heuristic: " << h << endl;
      cout << "remaining keys: { ";
      for (auto key : n_keys) {
        cout << key << " ";
      }
      cout << "}" << endl;
      cout << "remaining doors: { ";
      for (auto door : n_doors) {
        cout << door << " ";
      }
      cout << "}" << endl;

      cout << "visible keys: ";
      for (auto kv : n_visible_key_dists) {
        cout << "(" << kv.first << ", " << kv.second << "), ";
      }
      cout << endl;
      */

      State next = {n_cost, n_cost + h, n_location, n_keys, n_doors, n_visible_key_dists};
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
  int R = maze.size(), C = maze.at(0).size();

  Coordinate start = {-1, -1};
  unordered_map<char, Coordinate> keys;
  unordered_map<char, Coordinate> doors;

  for (int r = 0; r < R; r++) {
    for (int c = 0; c < C; c++) {
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
  Input input = {R, C, maze, keys, doors};

  int path_len = a_star(input, start);
  cout << "Part 1: " << path_len << endl;
}
