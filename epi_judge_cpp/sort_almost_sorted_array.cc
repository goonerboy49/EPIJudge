#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
vector<int> SortApproximatelySortedData(
    vector<int>::const_iterator sequence_begin,
    const vector<int>::const_iterator& sequence_end, int k) {
  std::priority_queue<int, std::vector<int>, std::greater<>> minHeap;

  vector<int> retVal;
  for (int i = 0; i < k && sequence_begin != sequence_end; i++) {
    minHeap.push(*sequence_begin);
    ++sequence_begin;
  }

  while(sequence_begin != sequence_end) {
    retVal.push_back(minHeap.top());
    minHeap.push(*sequence_begin);
    ++sequence_begin;
    minHeap.pop();
  }

  while(!minHeap.empty()) {
    retVal.push_back(minHeap.top());
    minHeap.pop();
  }

  return retVal;
}
vector<int> SortApproximatelySortedDataWrapper(const vector<int>& sequence,
                                               int k) {
  return SortApproximatelySortedData(cbegin(sequence), cend(sequence), k);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sequence", "k"};
  return GenericTestMain(
      args, "sort_almost_sorted_array.cc", "sort_almost_sorted_array.tsv",
      &SortApproximatelySortedDataWrapper, DefaultComparator{}, param_names);
}
