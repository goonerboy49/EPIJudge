#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int ComputeMaxRevenue(const vector<int>& coins, int a, int b, vector<vector<int>>& dp) {
  if (a > b) {
    return 0;
  }

  if (dp[a][b] == 0) {
    int pick_a = coins[a] + std::min(ComputeMaxRevenue(coins, a+2, b, dp), ComputeMaxRevenue(coins, a+1, b-1, dp));
    int pick_b = coins[b] + std::min(ComputeMaxRevenue(coins, a, b-2, dp), ComputeMaxRevenue(coins, a+1, b-1, dp));
    dp[a][b] = std::max(pick_a, pick_b);
  }

  return dp[a][b];
}

int MaximumRevenue(const vector<int>& coins) {
  vector<vector<int>> dp(coins.size(), vector<int>(coins.size(), 0));

  return ComputeMaxRevenue(coins, 0, coins.size()-1, dp);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"coins"};
  return GenericTestMain(args, "picking_up_coins.cc", "picking_up_coins.tsv",
                         &MaximumRevenue, DefaultComparator{}, param_names);
}
