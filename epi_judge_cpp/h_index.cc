#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
int HIndex(vector<int> citations) {
  // Sort the list
  std::sort(citations.begin(), citations.end());
  const size_t len = citations.size();
  for (int i = 0; i < len; i++) {
    int numRemaining = len - i;
    if (citations[i] >= numRemaining) {
      return numRemaining;
    }
  }

  return 0;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"citations"};
  return GenericTestMain(args, "h_index.cc", "h_index.tsv", &HIndex,
                         DefaultComparator{}, param_names);
}
