#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

/*
 * Notes
 * Create an empty subset and add each possible candidate elements to
 * the empty subset. Then keep expanding the subset size by expanding the
 * individual already added to the answer
 */

vector<vector<int>> GeneratePowerSet(const vector<int>& input_set) {
  vector<vector<int>> retVal(1, vector<int>());

  for (int i = 0; i < input_set.size(); i++) {
    int n = retVal.size();
    for(int j = 0; j < n; j++) {
      retVal.push_back(retVal[j]);
      retVal.back().push_back(input_set[i]);
    }
  }

  return retVal;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"input_set"};
  return GenericTestMain(
      args, "power_set.cc", "power_set.tsv", &GeneratePowerSet,
      &UnorderedComparator<vector<vector<int>>>, param_names);
}
