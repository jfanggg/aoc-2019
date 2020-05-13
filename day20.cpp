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

struct Node {
  int level;
  Coordinate coord;
  int r, c;

  Node(int level, Coordinate coord): level(level), coord(coord) {
    r = coord.first;
    c = coord.second;
  }

  bool operator==(const Node& other) const {
    return level == other.level && coord == other.coord;
  }
};

struct node_hash_fn
{
  std::size_t operator() (const Node& node) const
  {
    std::size_t seed = 0;
    boost::hash_combine(seed, node.level);
    boost::hash_combine(seed, node.coord);
    return seed;
  }
};

bool is_letter(char c) {
  return 'A' <= c && c <= 'Z';
}

int bfs(const vector<string>& maze, 
    const unordered_map<string, vector<Coordinate>>& portals,
    const bool& recursive) {
  
  int R = maze.size();
  int C = maze.at(0).size();

  Coordinate start = portals.at("AA").at(0);
  Coordinate end = portals.at("ZZ").at(0);

  unordered_map<Node, int, node_hash_fn> dists;
  queue<Node> q;
  q.push({0, start});
  while (!q.empty()) {
    Node current = q.front();
    q.pop();

    // got to the end
    if (current.coord == end && current.level == 0)
      break;

    for (int i = 0; i < 4; i++) {
      Node next = {current.level, {current.r + dr[i], current.c + dc[i]}};

      vector<char> neighbors;
      neighbors.push_back(maze.at(next.r).at(next.c));
      neighbors.push_back(maze.at(next.r + dr[i]).at(next.c + dc[i]));

      if (neighbors.at(0) == '#' || neighbors.at(0) == ' ') 
        continue;

      if (is_letter(neighbors.at(0))) {
        int start = (int) i > 1;
        string portal_name = string() + neighbors.at(start) + neighbors.at(1 - start);
        if (portal_name == "AA" || portal_name == "ZZ")
          continue;

        bool outer = current.r == 2 || current.r == R - 3;
        outer |= current.c == 2 || current.c == C - 3;

        if (outer && current.level == 0)
          continue;

        int next_level = current.level;
        if (recursive) {
          next_level += outer ? -1 : 1;
        }
        int portal_idx = (portals.at(portal_name).at(0) == current.coord) ? 1 : 0;

        next = {next_level, portals.at(portal_name).at(portal_idx)};
      }

      if (dists.find(next) != dists.end())
        continue;
      dists[next] = dists[current] + 1;
      q.push(next);
    }
  }
  return dists.at({0, end});
}

int main() {
  string s;
  vector<string> maze;
  unordered_map<string, vector<Coordinate>> portals;
  
  while (getline(cin, s)) {
    maze.push_back(s);
  }
  for (int r = 0; r < maze.size(); r++) {
    for (int c = 0; c < maze.at(0).size(); c++) {
      char letter = maze.at(r).at(c);
      if (letter != '.') 
        continue;
        
      for (int i = 0; i < 4; i++) {
        vector<char> neighbors;
        neighbors.push_back(maze.at(r + dr[i]).at(c + dc[i]));
        neighbors.push_back(maze.at(r + 2 * dr[i]).at(c + 2 * dc[i]));

        if (is_letter(neighbors.at(0))) {
          int start = (int) i > 1;
          string portal_name = string() + neighbors.at(start) + neighbors.at(1 - start);
          if (portals.find(portal_name) == portals.end()) {
            portals[portal_name] = {};
          }
          portals[portal_name].push_back({r, c});
        }
      }
    }
  }

  cout << "Part 1: " << bfs(maze, portals, 0) << endl;
  cout << "Part 2: " << bfs(maze, portals, 1) << endl;
}