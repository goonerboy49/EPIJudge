#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
double BuyAndSellStockTwice(const vector<double>& prices) {
  // Create an array such that each elem[i] represents highest profit if a transaction completes before or on i.
  double minSoFar = std::numeric_limits<double>::max();
  double maxProfit = 0;
  vector<double> firstBuys(prices.size(), 0.0);

  int i = 0;
  for (auto price : prices) {
    minSoFar = std::min(minSoFar, price);
    maxProfit = std::max(maxProfit, price-minSoFar);
    firstBuys[i++] = maxProfit;
  }

  // Work backwards by finding the second buy starting at i given the first transaction is completed by i-1
  double maxSoFar = std::numeric_limits<double>::min();
  for(int i = prices.size()-1; i > 0; i--) {
    maxSoFar = std::max(maxSoFar, prices[i]);
    maxProfit = std::max(maxProfit, firstBuys[i-1] + maxSoFar - prices[i]);
  }

  return maxProfit;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"prices"};
  return GenericTestMain(args, "buy_and_sell_stock_twice.cc",
                         "buy_and_sell_stock_twice.tsv", &BuyAndSellStockTwice,
                         DefaultComparator{}, param_names);
}
