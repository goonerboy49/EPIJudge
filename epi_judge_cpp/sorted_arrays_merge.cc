#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
struct ArrayIterators {
  bool operator>(const ArrayIterators& other) {
    return *current > *other.current;

  }

  std::vector<int>::const_iterator current;
  std::vector<int>::const_iterator end;
};

vector<int> MergeSortedArrays(const vector<vector<int>>& sorted_arrays) {
  vector<int> result;
  std::priority_queue<ArrayIterators, std::vector<ArrayIterators>, std::greater<>> minHeap;

  for (const auto& array : sorted_arrays) {
    if (!array.empty()) {
      minHeap.emplace(ArrayIterators{array.cbegin(), array.cend()});
    }
  }

  while(!minHeap.empty()) {
    ArrayIterators top = minHeap.top();
    result.emplace_back(*top.current);
    minHeap.pop();

    if (std::next(top.current) != top.end) {
      minHeap.emplace(ArrayIterators{std::next(top.current), top.end});
    }
  }

  return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sorted_arrays"};
  return GenericTestMain(args, "sorted_arrays_merge.cc",
                         "sorted_arrays_merge.tsv", &MergeSortedArrays,
                         DefaultComparator{}, param_names);
}
