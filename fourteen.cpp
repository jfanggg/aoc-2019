#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

typedef pair<string, int> Ingredient;

int main() {
  unordered_map<string, vector<Ingredient>> requires;     // chemical -> what ingredients it requires
  unordered_map<string, unordered_set<string>> produces;  // chemical -> which ingredients require it
  unordered_map<string, int> amount;                      // chemical -> how much you can produce at once
  unordered_map<string, int> req_amounts;                 // chemical -> how much of it you need

  int quantity;
  string chemical;
  while (cin >> quantity >> chemical) {

    // read in the requirements
    vector<Ingredient> ingredients;
    while (chemical[chemical.length() - 1] == ',') {
      chemical = chemical.substr(0, chemical.length() - 1);
      ingredients.push_back({chemical, quantity});
      cin >> quantity >> chemical;
    }
    ingredients.push_back({chemical, quantity});

    // read in the product
    cin >> chemical >> quantity >> chemical;

    for (Ingredient i : ingredients) {
      if (produces.find(i.first) == produces.end()) {
        produces[i.first] = unordered_set<string>();
      }
      produces[i.first].insert(chemical);
    }
    requires[chemical] = ingredients;
    amount[chemical] = quantity;
  }

  queue<string> unblocked;
  unblocked.push("FUEL");
  req_amounts["FUEL"] = 1;

  while (1) {
    string product = unblocked.front();
    unblocked.pop();
    if (product == "ORE") {
      break;
    }

    int req_amount = req_amounts.at(product);
    int multiplier = ceil(req_amount / (float) amount.at(product));
    for (Ingredient i : requires.at(product)) {
      string chemical = i.first;
      if (req_amounts.find(chemical) == req_amounts.end()) {
        req_amounts[chemical] = 0;
      }
      req_amounts[chemical] += multiplier * i.second;
      
      produces.at(chemical).erase(product);
      if (produces.at(chemical).size() == 0) {
        unblocked.push(chemical);
      }
    }
  }
  cout << "Part 1: " << req_amounts.at("ORE") << endl;
}
