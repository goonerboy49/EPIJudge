#include <vector>
#include <unordered_map>
#include "test_framework/generic_test.h"
using std::vector;

int LongestSubarrayWithDistinctEntries(const vector<int>& A) {
  std::unordered_map<int, int> elemToIndexMap;
  
  int startIdx = 0;
  int maxLen = 0;
  for(int i = 0; i < A.size(); i++) {
    auto iter = elemToIndexMap.find(A[i]);
    
    if (iter != elemToIndexMap.end()) {
      if (iter->second >= startIdx) {
        maxLen = std::max(maxLen, i - startIdx);
        startIdx = iter->second + 1;
      }
      iter->second = i;
    } else {
      elemToIndexMap[A[i]] = i;
    }
  }

  return std::max(maxLen, int(A.size() - startIdx));
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "longest_subarray_with_distinct_values.cc",
                         "longest_subarray_with_distinct_values.tsv",
                         &LongestSubarrayWithDistinctEntries,
                         DefaultComparator{}, param_names);
}
