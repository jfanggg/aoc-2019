#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
using namespace std;

typedef pair<int, int> Coordinate;
const int dr[4] = {1, 0, -1, 0};
const int dc[4] = {0, 1, 0, -1};
int dists[100][100];  // must be at least as large as the board

struct Input {
  int R, C;
  vector<string> maze;
  unordered_map<char, Coordinate> keys;
  unordered_map<char, Coordinate> doors;

  Input(int R, int C, vector<string> maze, unordered_map<char, Coordinate> keys, 
      unordered_map<char, Coordinate> doors):
    R(R), C(C), maze(maze), keys(keys), doors(doors) {}
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

unordered_map<char, int> bfs(const Input& input, const Coordinate& start) {
  memset(dists, -1, 100 * 100 * sizeof(dists[0][0]));
  unordered_map<char, int> dist_map;
  queue<Coordinate> q;

  dists[start.first][start.second] = 0;
  q.push(start);
  while (!q.empty()) {
    Coordinate coord = q.front();
    int r = coord.first;
    int c = coord.second;
    q.pop();

    for (int i = 0; i < 4; i++) {
      int new_r = r + dr[i];
      int new_c = c + dc[i];

      if (0 <= new_r && new_r < input.R && 0 <= new_c && new_c < input.C) {
        if (dists[new_r][new_c] != -1) {
          continue;
        }
        dists[new_r][new_c] = dists[r][c] + 1;

        char letter = input.maze.at(new_r).at(new_c);
        if (letter == '#' || is_upper(letter)) {
          continue;
        }

        if (is_lower(letter)) {
          dist_map[letter] = dists[new_r][new_c];
        }
        q.push({new_r, new_c});
      }
    }
  }
  return dist_map;
}

void dfs(Input& input, Coordinate start, int path_len, int& max_path, int level) {
  // cout << "Level " << level << " DFS at: (" << start.first << ", " << start.second << "), path len is: " << path_len << endl;
  /*
  for (string s : input.maze) {
    cout << s << endl;
  }
  */
  if (path_len >= max_path) {
    cout << "Pruning early" << endl;
    return;
  }

  auto dist_map = bfs(input, start);
  if (dist_map.empty()) {
    max_path = path_len; 
    cout << "Found all keys. New max_path: " << max_path << endl;
    return;
  } 

  int max_dist = 0;
  for (auto iter : dist_map) {
    max_dist = max(max_dist, iter.second);
  }
  if (path_len + max_dist >= max_path) {
    return;
  }

  for (auto iter : dist_map) {
    char letter = iter.first;
    int dist = iter.second;
    // cout << "Found key " << letter << " with dist: " << dist << endl;

    // acquire key and unlock door
    Coordinate key = input.keys.at(letter);
    Coordinate door = {-1, -1};
    if (input.doors.find(letter) != input.doors.end()) {
      door = input.doors.at(letter);
    }

    input.maze[key.first][key.second] = '.';
    if (door.first != -1) {
      input.maze[door.first][door.second] = '.';
    }

    dfs(input, key, path_len + dist, max_path, level + 1);
    
    // add key and door back
    input.maze[key.first][key.second] = letter;
    if (door.first != -1) {
      input.maze[door.first][door.second] = to_upper(letter);
    }
  }
}

int main() {
  int max_path = INT_MAX; 
  string s;
  vector<string> maze;
  
  while (cin >> s) {
    maze.push_back(s);
  }
  int R = maze.size(), C = maze.at(0).size();

  Coordinate start;
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

  dfs(input, start, 0, max_path, 0);
  cout << "Part 1: " << max_path << endl;
}
