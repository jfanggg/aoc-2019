#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <boost/functional/hash.hpp>
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
  string id;
  Coordinate location;
  unordered_set<char> remain_keys;
  unordered_map<char, int> visible_key_dists; 

  State(const Input& input, int cost, string id,
    Coordinate location, unordered_set<char> remain_keys)
      : cost(cost)
      , id(id)
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

string state_id(const Coordinate& coord, const unordered_set<char>& remain_keys) {
  string id = to_string(coord.first) + "," + to_string(coord.second) + ":";
  for (char c = 'a'; c <= 'z'; c++) {
    if (remain_keys.find(c) != remain_keys.end()) {
      id += c;
    }
  }
  return id;
}

// BFS to find visible keys and heuristic using remaining keys
void evaluate(const Input& input, const Coordinate& loc, 
    const unordered_set<char>& remain_keys, 
    unordered_map<char, int>& visible_key_dists, int& heuristic) {

  memset(dists, -1, 100 * 100 * sizeof(dists[0][0]));

  unordered_set<Coordinate, boost::hash<Coordinate>> blocked;
  unordered_map<Coordinate, Coordinate, boost::hash<Coordinate>> parent;
  unordered_map<char, Coordinate> remain_keys_coords;

  queue<Coordinate> q;
  dists[loc.first][loc.second] = 0;
  q.push(loc);
  while (!q.empty()) {
    Coordinate coord = q.front();
    q.pop();

    int r = coord.first;
    int c = coord.second;
    bool is_blocked = blocked.find(coord) != blocked.end();

    for (int i = 0; i < 4; i++) {
      int new_r = r + dr[i];
      int new_c = c + dc[i];
      Coordinate new_coord = {new_r, new_c};
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
        blocked.insert(new_coord);
      }

      // found a key. Record if it's a remaining or visible
      if (is_key(letter)) {
        if (remain_keys.find(letter) != remain_keys.end()) {
          if (!is_blocked) {
            visible_key_dists[letter] = dists[new_r][new_c];
          }
          remain_keys_coords[letter] = new_coord;
        }
      }
      parent[new_coord] = coord;
      q.push({new_r, new_c});
    }
  }

  // compute heuristic using remain_keys_coords
  if (remain_keys_coords.empty()) {
    heuristic = 0;
  } else {
    unordered_set<Coordinate, boost::hash<Coordinate>> seen;
    for (auto kv : remain_keys_coords) {
      auto coord = kv.second;

      seen.insert(coord);
      while (parent.find(coord) != parent.end()) {
        coord = parent.at(coord);
        seen.insert(coord);
      }
    }
    // number of nodes to visit to get to all keys, excluding start
    heuristic = seen.size() - 1;
  }
}

int a_star(const Input& input, const Coordinate& start_coord) {
  unordered_set<char> remain_keys;
  for (auto kv : input.keys) {
    remain_keys.insert(kv.first);
  }
  string start_id = state_id(start_coord, remain_keys);
  State start = {input, 0, start_id, start_coord, remain_keys};

  // seen states -> smallest cost to get there
  unordered_map<string, int> seen;

  auto comp = [](State a, State b) { return a.total_heuristic > b.total_heuristic; };
  priority_queue<State, vector<State>, decltype(comp)> q(comp);
  q.push(start);

  while(!q.empty()) {
    State current = q.top();
    q.pop();

    // found the answer
    if (current.remain_keys.empty())
     return current.cost;

    if (seen.find(current.id) != seen.end() && current.cost > seen.at(current.id))
      continue;
    seen[current.id] = current.cost;

    for (auto kv : current.visible_key_dists) {
      char key = kv.first;
      int cost = kv.second;

      auto next_cost = current.cost + cost;
      auto next_location = input.keys.at(key);
      auto next_keys = current.remain_keys;
      next_keys.erase(key);

      string next_id = state_id(next_location, next_keys);
      if (seen.find(next_id) != seen.end() && next_cost >= seen.at(next_id))
        continue;
      seen[next_id] = next_cost;

      State next = {input, next_cost, next_id, next_location, next_keys};
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