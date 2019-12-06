#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
using namespace std;
const string INVALID_ID = ")";

unordered_map<string, string> parent;   // ID of node -> ID of node's parent
unordered_map<string, int> dist;        // distance from YOU

string get_parent(string p) {
  return parent.find(p) != parent.end() ? parent[p] : INVALID_ID;
}

int main() {
  string s;

  while (cin >> s) {
    size_t split = s.find(")");
    string a = s.substr(0, split); 
    string b = s.substr(split + 1); 

    parent[b] = a;
    dist[a] = -1;
  }

  // Traverse to the root from every node
  int ans1 = 0;
  for (auto const& itr : parent) {
    string p = itr.second;
    while (p != INVALID_ID) {
      ans1 += 1;
      p = get_parent(p);
    }
  }
  cout << "Part 1: " << ans1 << endl;

  string cur = "YOU";
  dist[cur] = 0;
  string p = get_parent(cur);

  while (p != INVALID_ID) {
    dist[p] = dist[cur] + 1;
    cur = p;
    p = get_parent(p);
  }
  
  int ans2 = 0;
  int dist_from_san = 0;
  p = get_parent("SAN");
  while (p != INVALID_ID) {
    dist_from_san++;

    // found the common ancestor
    if (dist[p] != -1) {
      ans2 = dist[p] + dist_from_san - 2;
      break;
    }
    p = get_parent(p);
  }
  cout << "Part 2: " << ans2 << endl;
}

