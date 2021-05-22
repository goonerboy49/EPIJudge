#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int FindMaximumSubarray(const vector<int>& A) {
  int maxSum = 0;
  int currSum = 0;
  for (int i = 0; i < A.size(); i++) {
    currSum = std::max(currSum + A[i], A[i]);
    maxSum = std::max(maxSum, currSum);
  }
  return maxSum;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "max_sum_subarray.cc", "max_sum_subarray.tsv",
                         &FindMaximumSubarray, DefaultComparator{},
                         param_names);
}
