#include <string>
#include <unordered_set>
#include "test_framework/generic_test.h"
using std::string;
using std::unordered_set;

// Uses BFS to find the least steps of transformation.
int TransformString(unordered_set<string> D, const string& s, const string& t) {
  struct step {
    std::string intermediate;
    int distance;
  };

  std::queue<step> steps;
  steps.emplace(step{s, 0});
  D.erase(s);

  while(!steps.empty()) {
    auto curr = steps.front();
    steps.pop();

    if (curr.intermediate == t) {
      return curr.distance;
    }

    string str =  curr.intermediate;
    for (int i = 0; i < str.size(); i++) {
      for (int j = 0; j < 26; j++) {
        str[i] = 'a' + j;
        // Now check whether this new string is present in the dictionary, if present add to 
        // the queue
        auto iter = D.find(str);
        if (iter != D.end()) {
          steps.emplace(step{str, curr.distance+1});
          D.erase(iter);
        }
      }
      str[i] = curr.intermediate[i];
    }
  }
  return -1;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"D", "s", "t"};
  return GenericTestMain(args, "string_transformability.cc",
                         "string_transformability.tsv", &TransformString,
                         DefaultComparator{}, param_names);
}
