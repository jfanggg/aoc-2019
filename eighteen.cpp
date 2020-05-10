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
  unordered_map<char, Coordinate> key_locations;

  Input(vector<string> maze, unordered_map<char, Coordinate> key_locations)
      : maze(maze)
      , key_locations(key_locations)
  {}
};

void evaluate(const Input& input, const Coordinate& loc, 
    const unordered_set<char>& remain_keys, 
    unordered_map<char, int>& all_visible_keys, int& heuristic);

struct State {
  int cost;
  string id;
  vector<Coordinate> locations;
  unordered_set<char> remain_keys;

  // cost + heuristic for remainder
  int total_heuristic;  

  // robot -> [key -> dist]
  vector<unordered_map<char, int>> all_visible_keys;

  State(const Input& input, int cost, string id,
    vector<Coordinate> locations, unordered_set<char> remain_keys)
      : cost(cost)
      , id(id)
      , locations(locations)
      , remain_keys(remain_keys) {
    
    int h;
    total_heuristic = cost;

    unordered_map<char, int> robot_visible_keys;

    for (auto location : locations) {
      evaluate(input, location, remain_keys, robot_visible_keys, h);
      total_heuristic += h;
      all_visible_keys.push_back(robot_visible_keys);
      robot_visible_keys.empty();
    }
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

string state_id(const vector<Coordinate>& coords, const unordered_set<char>& remain_keys) {
  string id = "";
  for (auto coord : coords) {
    id += to_string(coord.first) + "," + to_string(coord.second) + ",";
  }
  id += ":";
  for (char c = 'a'; c <= 'z'; c++) {
    if (remain_keys.find(c) != remain_keys.end()) {
      id += c;
    }
  }
  return id;
}

// BFS to find visible keys and heuristic using remaining keys
void evaluate(const Input& input, const Coordinate& start_location, 
    const unordered_set<char>& remain_keys, 
    unordered_map<char, int>& all_visible_keys, int& heuristic) {

  memset(dists, -1, 100 * 100 * sizeof(dists[0][0]));

  unordered_set<Coordinate, boost::hash<Coordinate>> blocked;
  unordered_map<Coordinate, Coordinate, boost::hash<Coordinate>> parent;
  unordered_map<char, Coordinate> remain_keys_coords;

  queue<Coordinate> q;
  
  dists[start_location.first][start_location.second] = 0;
  q.push(start_location);
  while (!q.empty()) {
    Coordinate location = q.front();
    q.pop();

    int r = location.first;
    int c = location.second;
    bool is_blocked = blocked.find(location) != blocked.end();

    for (int i = 0; i < 4; i++) {
      int new_r = r + dr[i];
      int new_c = c + dc[i];
      Coordinate new_location = {new_r, new_c};
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
        blocked.insert(new_location);
      }

      // found a key. Record if it's a remaining or visible
      if (is_key(letter)) {
        if (remain_keys.find(letter) != remain_keys.end()) {
          if (!is_blocked) {
            all_visible_keys[letter] = dists[new_r][new_c];
          }
          remain_keys_coords[letter] = new_location;
        }
      }
      parent[new_location] = location;
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

int a_star(const Input& input, const vector<Coordinate>& start_coord) {
  unordered_set<char> remain_keys;
  for (auto kv : input.key_locations) {
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

    for (int robot_id = 0; robot_id < current.all_visible_keys.size(); robot_id++) {
      auto robot_visible_keys = current.all_visible_keys.at(robot_id);
      for (auto kv : robot_visible_keys) {
        char key = kv.first;
        int cost = kv.second;

        auto next_cost = current.cost + cost;
        auto next_locations = current.locations;
        next_locations[robot_id] = input.key_locations.at(key);
        auto next_keys = current.remain_keys;
        next_keys.erase(key);

        string next_id = state_id(next_locations, next_keys);
        if (seen.find(next_id) != seen.end() && next_cost >= seen.at(next_id))
          continue;
        seen[next_id] = next_cost;

        State next = {input, next_cost, next_id, next_locations, next_keys};
        q.push(next);
      }
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
  vector<Coordinate> starts;
  unordered_map<char, Coordinate> keys;

  for (int r = 0; r < maze.size(); r++) {
    for (int c = 0; c < maze.at(0).size(); c++) {
      char letter = maze.at(r).at(c);
      if (letter == '@') {
        starts.push_back({r, c});
        maze[r][c] = '.';
      } else if (is_key(letter)) {
        keys[letter] = {r, c};
      }
    }
  }
  Input input = {maze, keys};

  int path_len = a_star(input, starts);
  cout << "Part 1: " << path_len << endl;

  /*
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      maze[start.first][start.second] = i * j == 0 ? '#' : '.';
    }
  }
  cout << "Part 2: " << path_len << endl;
  */
}