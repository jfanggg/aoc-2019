#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

const long TRIL = 1E12;
typedef pair<string, int> Requirement;  // chemical name, amount required

struct Ingredient {
  vector<Requirement> requires;         // what chemical and amounts it requires
  unordered_set<string> produces;       // which ingredients require it
  int produce_amount;                   // how much you can produce at once
  unordered_set<string> blockers;       // copy of produces

  void reset_blockers() {
    blockers = produces;
  } 
};

long solve(unordered_map<string, Ingredient>& ingredients, long num_fuel) {
  unordered_map<string, long> req_amounts;                 // chemical -> how much of it you must produce
  for (auto& iter : ingredients) {
    iter.second.reset_blockers();
  }

  queue<string> unblocked;
  unblocked.push("FUEL");
  req_amounts["FUEL"] = num_fuel;

  while (1) {
    string p_chemical = unblocked.front();
    unblocked.pop();
    if (p_chemical == "ORE") {
      break;
    }

    Ingredient& p_ingredient = ingredients.at(p_chemical);
    long p_req_amount = req_amounts.at(p_chemical);
    long multiplier = ceil(p_req_amount / (float) p_ingredient.produce_amount);

    for (Requirement req : p_ingredient.requires) {
      string r_chemical = req.first;
      req_amounts[r_chemical] += multiplier * req.second;
      
      Ingredient& r_ingredient = ingredients.at(r_chemical);
      r_ingredient.blockers.erase(p_chemical);
      if (r_ingredient.blockers.size() == 0) {
        unblocked.push(r_chemical);
      }
    }
  }
  return req_amounts.at("ORE");
}

int main() {
  unordered_map<string, Ingredient> ingredients;          // chemical -> ingrdient info

  int quantity;
  string chemical;
  while (cin >> quantity >> chemical) {

    // read in the requirements
    vector<Requirement> reqs;
    while (chemical[chemical.length() - 1] == ',') {
      chemical = chemical.substr(0, chemical.length() - 1);
      reqs.push_back({chemical, quantity});
      cin >> quantity >> chemical;
    }
    reqs.push_back({chemical, quantity});

    // read in the product
    cin >> chemical;  // get rid of =>
    cin >> quantity >> chemical;

    ingredients[chemical].produce_amount = quantity;
    ingredients[chemical].requires = reqs;

    for (Requirement r : reqs) {
      ingredients[r.first].produces.insert(chemical);
    }
  }
  cout << "Part 1: " << solve(ingredients, 1) << endl;

  long lo = 1, hi = 1E12, mid = 0;
  while (lo < hi) {
    mid = ceil((lo + hi) / 2.0);
    long ores = solve(ingredients, mid);
    if (ores > TRIL) {
      hi = mid - 1;
    } else {
      lo = mid;
    }
  }
  cout << "Part 2: " << lo << endl;
}
