#include <iostream>
#include <string>
#include <vector>
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

  for (int t = 0; t < 1000; t++) {
    // apply gravity
    for (int i = 0; i < moons.size(); i++) {
      Moon& A = moons.at(i);
      for (int j = i + 1; j < moons.size(); j++) {
        Moon& B = moons.at(j);
        for (int axis = 0; axis < 3; axis++) {
          if (A.pos[axis] > B.pos[axis]) {
            A.vel[axis]--;
            B.vel[axis]++;
          } else if (A.pos[axis] < B.pos[axis]) {
            A.vel[axis]++;
            B.vel[axis]--;
          }
        }
      }
    }
    // apply velocity
    for (int i = 0; i < moons.size(); i++) {
      Moon& A = moons.at(i);
      for (int axis = 0; axis < 3; axis++) {
        A.pos[axis] += A.vel[axis];
      }
    }
  }
  int ans1 = 0;
  for (Moon m : moons) {
    ans1 += m.energy();
  }
  cout << "Part 1: " << ans1 << endl;
}

