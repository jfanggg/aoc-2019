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

bool is_letter(char c) {
  return 'A' <= c && c <= 'Z';
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

  Coordinate start = portals.at("AA").at(0);
  Coordinate end = portals.at("ZZ").at(0);

  unordered_map<Coordinate, int, boost::hash<Coordinate>> dists;
  queue<Coordinate> q;
  q.push(start);
  while (!q.empty()) {
    Coordinate current = q.front();
    q.pop();

    // got to the next
    if (current == end)
      break;

    for (int i = 0; i < 4; i++) {
      Coordinate next = {current.first + dr[i], current.second + dc[i]};
      vector<char> neighbors;
      neighbors.push_back(maze.at(next.first).at(next.second));
      neighbors.push_back(maze.at(next.first + dr[i]).at(next.second + dc[i]));

      if (neighbors.at(0) == '#' || neighbors.at(0) == ' ') 
        continue;

      if (is_letter(neighbors.at(0))) {
        int start = (int) i > 1;
        string portal_name = string() + neighbors.at(start) + neighbors.at(1 - start);
        if (portal_name == "AA" || portal_name == "ZZ")
          continue;

        int idx = (portals.at(portal_name).at(0) == current) ? 1 : 0;
        next = portals.at(portal_name).at(idx);
      }

      if (dists.find(next) != dists.end())
        continue;
      dists[next] = dists[current] + 1;
      q.push(next);
    }
  }
  cout << "Part 1: " << dists.at(end) << endl;
}