#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;

/**
 * Notes -
 * The main catch is number of columns that represent the capacity should
 * be capacity+1 because index 0 should represent the scenario when the 
 * sack is full.
 */ 

struct Item {
  int weight, value;
};

int KnapsackRecurse(vector<vector<int>>& dp, int offset, int capacity, vector<Item> const& items) {
  if (offset < 0) {
    return 0;
  }

  if (dp[offset][capacity] == -1) {
    int withItem = items[offset].weight <= capacity ? items[offset].value + KnapsackRecurse(dp, offset -1, capacity - items[offset].weight, items) : 0;
    int withoutItem = KnapsackRecurse(dp, offset-1, capacity, items);
    dp[offset][capacity] = std::max(withItem, withoutItem);
  }

  return dp[offset][capacity];
}


int OptimumSubjectToCapacity(const vector<Item>& items, int capacity) {
  vector<vector<int>> dp(items.size(), vector<int>(capacity+1, -1));

  return KnapsackRecurse(dp, items.size()-1, capacity, items);
}
template <>
struct SerializationTraits<Item> : UserSerTraits<Item, int, int> {};

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"items", "capacity"};
  return GenericTestMain(args, "knapsack.cc", "knapsack.tsv",
                         &OptimumSubjectToCapacity, DefaultComparator{},
                         param_names);
}
