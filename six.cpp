#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
using namespace std;

unordered_map<string, string> parent;
unordered_map<string, vector<string>> children;
unordered_map<string, int> dist;

int main() {
  string s;

  while (cin >> s) {
    size_t split = s.find(")");
    string a = s.substr(0, split); 
    string b = s.substr(split + 1, 7); 

    if (children.find(a) == children.end()) {
      children[a] = vector<string>();
    }
    parent[b] = a;
    children[a].push_back(b);

    dist[a] = -1;
    dist[b] = -1;
  }

  // Traverse to the root from every node
  int ans1 = 0;
  for (auto const& itr : parent) {
    string p = itr.second;
    while (p != "-") {
      ans1 += 1;
      p = parent.find(p) != parent.end() ? parent[p] : "-";
    }
  }
  cout << "Part 1: " << ans1 << endl;

  // standard BFS to get distance from YOU to SAN
  queue<string> q;
  q.push("YOU");
  dist["YOU"] = 0;

  while (!q.empty()) {
    string cur = q.front(); q.pop();

    if (parent.find(cur) != parent.end() && dist[parent[cur]] == -1) {
      dist[parent[cur]] = dist[cur] + 1;
      q.push(parent[cur]);
    }

    if (children.find(cur) == children.end()) 
      continue;
    for (string child : children[cur]) {
      if (dist[child] == -1) {
        dist[child] = dist[cur] + 1;
        q.push(child);
      }
    }
  }
  cout << "Part 2: " << dist["SAN"] - 2 << endl;
}

