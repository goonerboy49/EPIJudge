#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

vector<double> OnlineMedian(vector<int>::const_iterator sequence_begin,
                            const vector<int>::const_iterator& sequence_end) {
  vector<double> retVal;
  std::priority_queue<int, std::vector<int>, std::greater<>> maxHeap;
  std::priority_queue<int, std::vector<int>, std::less<>> minHeap;

  while(sequence_begin != sequence_end) {
    minHeap.push(*sequence_begin);
    maxHeap.push(minHeap.top());
    minHeap.pop();

    if (maxHeap.size() > minHeap.size()) {
      minHeap.push(maxHeap.top());
      maxHeap.pop();
    }

    // Add to the result
    retVal.push_back(maxHeap.size() == minHeap.size() ? 0.5 * (minHeap.top() + maxHeap.top()): minHeap.top());
    ++sequence_begin;
  }

  return retVal;
}
vector<double> OnlineMedianWrapper(const vector<int>& sequence) {
  return OnlineMedian(cbegin(sequence), cend(sequence));
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sequence"};
  return GenericTestMain(args, "online_median.cc", "online_median.tsv",
                         &OnlineMedianWrapper, DefaultComparator{},
                         param_names);
}
