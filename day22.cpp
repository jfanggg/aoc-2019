#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <boost/algorithm/string.hpp>

using namespace std;
typedef vector<long> Deck;
typedef pair<int, long> Command;
const int CUT = 0;
const int REVERSE = 1;
const int DEAL_INCREMENT = 2;

long do_command(Command command, long deck_size, long x) {
  long n = command.second;
  switch (command.first) {
    case CUT:
      n = (n + deck_size) % deck_size;
      return (x - n + deck_size) % deck_size;
    case REVERSE:
      return deck_size - 1 - x;
    case DEAL_INCREMENT:
      return (x * n) % deck_size;
    default:
      return -1;
  }
}

int main() {
  string s;
  vector<string> lines;
  vector<Command> commands;
  while (getline(cin, s)) {
    lines.push_back(s);
  }
  for (string line : lines) {
    vector<string> words;
    boost::split(words, line, boost::is_any_of(" "));
    string first = words.front();
    string last = words.back();

    if (first == "cut") {
      commands.push_back({CUT, stoi(last)});
    } else if (first == "deal" && last == "stack") {
      commands.push_back({REVERSE, 0});
    } else if (first == "deal") {
      commands.push_back({DEAL_INCREMENT, stoi(last)});
    }
  }

  Deck deck;
  long deck_size = 10007;
  long x = 2019;
  for (auto command : commands) {
    x = do_command(command, deck_size, x);
  }

  cout << "Part 1: " << x << endl;
}