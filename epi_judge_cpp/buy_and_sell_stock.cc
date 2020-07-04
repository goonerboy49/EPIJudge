#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
double BuyAndSellStockOnce(const vector<double>& prices) {
  double minSofar = std::numeric_limits<double>::max();
  double maxProfit = 0;
  for (auto price : prices) {
    minSofar = std::min(minSofar, price);
    maxProfit = std::max(maxProfit, price-minSofar);
  }

  return maxProfit;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"prices"};
  return GenericTestMain(args, "buy_and_sell_stock.cc",
                         "buy_and_sell_stock.tsv", &BuyAndSellStockOnce,
                         DefaultComparator{}, param_names);
}