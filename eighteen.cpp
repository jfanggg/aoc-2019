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

struct State {
  int cost;
  int total_heuristic = -1;  // cost + heuristic for remainder
  Coordinate location;
  unordered_set<char> remain_keys;
  unordered_set<char> remain_doors;

  State(int cost, int total_heuristic, Coordinate location,
    unordered_set<char> keys, unordered_set<char> doors)
      : cost(cost)
      , total_heuristic(total_heuristic)
      , location(location)
      , remain_keys(keys)
      , remain_doors(doors) 
  {}
};

bool is_lower(char c) {
  return 'a' <= c && c <= 'z';
}
bool is_upper(char c) {
  return 'A' <= c && c <= 'Z';
}
char to_upper(char c) {
  return c - 'a' + 'A';
}
char to_lower(char c) {
  return c - 'A' + 'a';
} 

// BFS to finds all the visible keys from a given state
unordered_map<char, int> look(const Input& input, State& state) {
  memset(dists, -1, 100 * 100 * sizeof(dists[0][0]));

  queue<Coordinate> q;
  unordered_map<char, int> keys;

  dists[state.location.first][state.location.second] = 0;
  q.push(state.location);
  while (!q.empty()) {
    Coordinate coord = q.front();
    int r = coord.first;
    int c = coord.second;
    q.pop();

    for (int i = 0; i < 4; i++) {
      int new_r = r + dr[i];
      int new_c = c + dc[i];

      if (!(0 <= new_r && new_r < input.R && 0 <= new_c && new_c < input.C)) 
        continue;
      if (dists[new_r][new_c] != -1) 
        continue;

      dists[new_r][new_c] = dists[r][c] + 1;
      char letter = input.maze.at(new_r).at(new_c);
      if (letter == '#')
        continue;
      if (is_upper(letter)) {
        char door = to_lower(letter);
        if (state.remain_doors.find(door) != state.remain_doors.end())
          continue;
      }
      
      if (is_lower(letter)) {
        keys[letter] = dists[new_r][new_c];
      }
      q.push({new_r, new_c});
    }
  }
  return keys;
}

// heuristic for A*
int heuristic(const Input& input, const Coordinate& location, 
    const unordered_set<char>& remain_keys) {

  int pos_x = 0, neg_x = 0;
  int pos_y = 0, neg_y = 0;

  for (char key: remain_keys) {
    auto coord = input.keys.at(key);
    pos_x = max(pos_x,  coord.first - location.first);
    neg_x = max(neg_x, -coord.first + location.first);
    pos_y = max(pos_y,  coord.second - location.second);
    neg_y = max(neg_y, -coord.second + location.second);
  }
  int h2 = pos_x + neg_x + pos_y + neg_y;

  /*
  int h = 0;
  for (char key: remain_keys) {
    auto key_coord = input.keys.at(key);
    h = max(h, abs(key_coord.first - location.first) + abs(key_coord.second - location.second));
  }
  */
  return h2;
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
  int h = heuristic(input, start, remain_keys);
  State s0 = {0, h, start, remain_keys, remain_doors};

  auto comp = [](State a, State b) { return a.total_heuristic > b.total_heuristic; };
  priority_queue<State, vector<State>, decltype(comp)> q(comp);
  q.push(s0);

  while(!q.empty()) {
    State current = q.top();
    q.pop();
    /*
    cout << "A*: loc = (" << current.location.first << ", " << current.location.second << ")" << endl;
    cout << "cost: " << current.cost << ", heuristic: " << current.total_heuristic << endl;
    cout << "remaining keys: { ";
    for (auto key : current.remain_keys) {
      cout << key << " ";
    }
    cout << "}" << endl;
    cout << "remaining doors: { ";
    for (auto door : current.remain_doors) {
      cout << door << " ";
    }
    cout << "}" << endl;
    */

    if (current.remain_keys.empty()) {
      return current.cost;
    }

    unordered_map<char, int> visible_keys = look(input, current);
    /*
    cout << "visible keys: ";
    for (auto kv : visible_keys) {
      cout << "(" << kv.first << ", " << kv.second << "), ";
    }
    cout << endl;
    */

    for (auto kv : visible_keys) {
      char key = kv.first;
      int cost = kv.second;

      auto new_cost = current.cost + cost;
      auto new_location = input.keys.at(key);
      auto new_keys = current.remain_keys;
      auto new_doors = current.remain_doors;
      new_keys.erase(key);
      new_doors.erase(key);
      int h = heuristic(input, new_location, new_keys);

      State next = {new_cost, new_cost + h, new_location, new_keys, new_doors};
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
      } else if (is_lower(letter)) {
        keys[letter] = {r, c};
      } else if (is_upper(letter)) {
        doors[to_lower(letter)] = {r, c};
      }
    }
  }
  Input input = {R, C, maze, keys, doors};

  int path_len = a_star(input, start);
  cout << "Part 1: " << path_len << endl;
}
