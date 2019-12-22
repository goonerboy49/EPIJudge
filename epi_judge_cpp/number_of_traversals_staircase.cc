#include "test_framework/generic_test.h"

int ComputeNumWays(int top, int maxStep, std::vector<int>& dp) {
  if (top <= 1) {
    return 1;
  }

  if (dp[top] == 0) {
    for (int i = 1; i <= maxStep && top - i >= 0; i++) {
      dp[top] += ComputeNumWays(top-i, maxStep, dp);
    }
  }
  return dp[top];
}


int NumberOfWaysToTop(int top, int maximum_step) {
  std::vector<int> dp(top+1, 0);

  return ComputeNumWays(top, maximum_step, dp);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"top", "maximum_step"};
  return GenericTestMain(args, "number_of_traversals_staircase.cc",
                         "number_of_traversals_staircase.tsv",
                         &NumberOfWaysToTop, DefaultComparator{}, param_names);
}
