#include <vector>
#include <algorithm>
#include "test_framework/generic_test.h"
using std::vector;

vector<int> ConstructCandidates(const vector<int>& A, vector<int> currAns) {
  std::vector<int> diff;
  std::sort(currAns.begin(), currAns.end());
  std::set_difference(A.begin(), A.end(), currAns.begin(), currAns.end(), std::back_inserter(diff));
  return diff;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "permutations.cc", "permutations.tsv", &Permutations,
      &UnorderedComparator<vector<vector<int>>>, param_names);
}
