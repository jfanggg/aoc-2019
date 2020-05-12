#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>

using namespace std;
const int DECK_SIZE = 10007;

void reverse(vector<int>& deck) {
  reverse(deck.begin(), deck.end());
}

void cut(vector<int>& deck, int n) {
  n = (n + DECK_SIZE) % DECK_SIZE;

  vector<int> new_deck(DECK_SIZE);
  copy(deck.begin() + n, deck.end(), new_deck.begin());

  int copied = DECK_SIZE - n;
  copy(deck.begin(), deck.begin() + n, new_deck.begin() + copied);

  deck = new_deck;
}

void deal_with_increment(vector<int>& deck, int n) {
  vector<int> new_deck(DECK_SIZE);
  for (int i = 0; i < deck.size(); i++) {
    new_deck[(i * n) % DECK_SIZE] = deck.at(i);
  }
  deck = new_deck;
}

int main() {
  string s;
  vector<string> commands;
  
  while (getline(cin, s)) {
    commands.push_back(s);
  }

  vector<int> deck;
  for (int i = 0; i < DECK_SIZE; i++) {
    deck.push_back(i);
  }

  for (string command : commands) {
    vector<string> words;
    boost::split(words, command, boost::is_any_of(" "));
    string first = words.front();
    string last = words.back();

    if (first == "cut") {
      int n = stoi(last);
      cut(deck, n);
    } else if (first == "deal" && last == "stack") {
      reverse(deck);
    } else if (first == "deal") {
      int n = stoi(last);
      deal_with_increment(deck, n);
    }
  }

  int ans1 = -1;
  for (int i = 0; i < DECK_SIZE; i++) {
    if (deck.at(i) == 2019) {
      ans1 = i;
    }
  }
  cout << "Part 1: " << ans1 << endl;
}