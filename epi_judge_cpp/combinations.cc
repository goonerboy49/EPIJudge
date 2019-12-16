#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

void RecurseCombinations(int idx, int n, int k, vector<int> *currAns, vector<vector<int>>& ans) 
{
  if (currAns->size() == k) {
    ans.emplace_back(*currAns);
    return;
  }

  const int num_remaining = k - currAns->size();
  for(int i = idx; i <= n && num_remaining <= n-i+1; i++) {
    currAns->push_back(i);
    RecurseCombinations(i+1, n, k, currAns, ans);
    currAns->pop_back();
  }
}

vector<vector<int>> Combinations(int n, int k) {
  vector<vector<int>> ans;
  auto currAns = std::make_unique<vector<int>>();

  RecurseCombinations(1, n, k, currAns.get(), ans);
  return ans;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n", "k"};
  return GenericTestMain(
      args, "combinations.cc", "combinations.tsv", &Combinations,
      &UnorderedComparator<std::vector<std::vector<int>>>, param_names);
}
