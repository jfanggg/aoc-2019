#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

struct Moon {
  vector<int> pos;
  vector<int> vel;

  Moon(int pos_x, int pos_y, int pos_z) {
    pos = {pos_x, pos_y, pos_z};
    vel = {0, 0, 0};
  }

  int kinetic_energy() {
    return abs(vel.at(0)) + abs(vel.at(1)) + abs(vel.at(2));
  }

  int potential_energy() {
    return abs(pos.at(0)) + abs(pos.at(1)) + abs(pos.at(2));
  }

  int energy() {
    return kinetic_energy() * potential_energy();
  }
};

void simulate_axis(int axis, vector<Moon>& moons) {
  // apply gravity
  for (int i = 0; i < moons.size(); i++) {
    for (int j = i + 1; j < moons.size(); j++) {
      Moon& A = moons.at(i);
      Moon& B = moons.at(j);

      if (A.pos[axis] > B.pos[axis]) {
        A.vel[axis]--;
        B.vel[axis]++;
      } else if (A.pos[axis] < B.pos[axis]) {
        A.vel[axis]++;
        B.vel[axis]--;
      }
    }
  }
  // apply velocity
  for (int i = 0; i < moons.size(); i++) {
    Moon& A = moons.at(i);
    A.pos[axis] += A.vel[axis];
  }
}

string serialize(int axis, vector<Moon>& moons) {
  string serialized = "";
  for (Moon& m : moons) {
    serialized += to_string(m.pos[axis]) + "," + to_string(m.vel[axis]) + ",";
  }
  return serialized;
}

int gcd(int a, int b) {
  if (a == 0) {
    return b;
  }
  return gcd(b % a, a);
}

int main() {
  string s;
  vector<int> read_in;
  vector<Moon> moons;
  while (cin >> s) {
    size_t start = s.find("=") + 1;
    int end = s.length() - 1;
    int val = stoi(s.substr(start, end - start));
    read_in.push_back(val);

    if (read_in.size() == 3) {
      moons.push_back({read_in.at(0), read_in.at(1), read_in.at(2)});
      read_in.clear();
    }
  }
  vector<Moon> initial_state = moons;

  for (int t = 0; t < 1000; t++) {
    for (int axis = 0; axis < 3; axis++) {
      simulate_axis(axis, moons);
    }
  }
  int ans1 = 0;
  for (Moon m : moons) {
    ans1 += m.energy();
  }
  cout << "Part 1: " << ans1 << endl;

  moons = initial_state;
  long ans2 = 1;
  for (int axis = 0; axis < 3; axis++) {
    unordered_set<string> seen;
    int cycle_len = 0;
    
    string state = serialize(axis, moons);
    seen.insert(state);
    int ctr = 0;
    while (1) {
      simulate_axis(axis, moons);
      cycle_len++;
      
      string state = serialize(axis, moons); 
      if (seen.find(state) != seen.end()) {
        ans2 = ans2 * cycle_len / gcd(ans2, cycle_len);
        break;
      }
      seen.insert(state);
      ctr++;
    }
  }
  cout << "Part 2: " << ans2 << endl;
}
