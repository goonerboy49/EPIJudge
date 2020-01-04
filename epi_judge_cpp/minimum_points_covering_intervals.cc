#include <algorithm>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;

struct Interval {
  int left, right;
};

int FindMinimumVisits(vector<Interval> intervals) {

  auto cmp = [](const Interval& a, const Interval& b) {
    return a.right < b.right;
  };

  std::sort(intervals.begin(), intervals.end(), cmp);

  int ans = 0;

  int currEnd = std::numeric_limits<int>::min();

  for(int start = 0; start < intervals.size(); start++) {
    if (intervals[start].left > currEnd) {
      currEnd = intervals[start].right;
      ++ans;
    }
  }

  return ans;
}
template <>
struct SerializationTraits<Interval> : UserSerTraits<Interval, int, int> {};

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"intervals"};
  return GenericTestMain(args, "minimum_points_covering_intervals.cc",
                         "minimum_points_covering_intervals.tsv",
                         &FindMinimumVisits, DefaultComparator{}, param_names);
}
