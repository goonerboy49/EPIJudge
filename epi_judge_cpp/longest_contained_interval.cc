#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int LongestContainedRange(const vector<int>& A) {
  std::unordered_set<int> ASet(std::begin(A), std::end(A));
  
  int maxLen = 0;
  while(!ASet.empty()) {
    int a = *ASet.begin();
    ASet.erase(a);

    int lower = a - 1;
    while(ASet.count(lower)) {
      ASet.erase(lower);
      --lower;
    }

    int upper = a + 1;
    while(ASet.count(upper)) {
      ASet.erase(upper);
      ++upper;
    }
    maxLen = std::max(maxLen, upper - lower - 1);
  }

  return maxLen;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "longest_contained_interval.cc", "longest_contained_interval.tsv",
      &LongestContainedRange, DefaultComparator{}, param_names);
}
