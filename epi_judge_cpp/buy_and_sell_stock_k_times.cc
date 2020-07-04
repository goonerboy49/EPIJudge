#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

double BuyAndSellStockKTimes(const vector<double>& prices, int k) {
  using ValleyPeak = std::pair<double, double>;
  std::stack<ValleyPeak> prevWindows;
  std::priority_queue<double> profits;

  int currIdx = 0;
  double currValley = prices[0];
  double currPeak = prices[0];
  while(currIdx < prices.size()) {
    // Find current valley
    while(currIdx < prices.size()-1 && prices[currIdx] >= prices[currIdx+1]) {
      ++currIdx;
    }

    currValley = prices[currIdx];

    // Find current peak
    while(currIdx < prices.size()-1 && prices[currIdx] < prices[currIdx+1]) {
      ++currIdx;
    }

    currPeak = prices[currIdx];
    while(!prevWindows.empty() && currValley < prevWindows.top().first) {
      profits.push(prevWindows.top().second - prevWindows.top().first);
      prevWindows.pop();
    }

    while(!prevWindows.empty() && currValley >= prevWindows.top().first && prevWindows.top().second < currPeak) {
      profits.push(prevWindows.top().second - currValley);
      currValley = prevWindows.top().first;
      prevWindows.pop();
    }

    prevWindows.push(std::make_pair(currValley, currPeak));
    ++currIdx;
  }

  while(!prevWindows.empty()) {
    profits.push(prevWindows.top().second-prevWindows.top().first);
    prevWindows.pop();
  }

  double maxProfit = 0.0;
  while(!profits.empty() && k) {
    maxProfit += profits.top();
    profits.pop();
    --k;
  }
  return maxProfit;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"prices", "k"};
  return GenericTestMain(
      args, "buy_and_sell_stock_k_times.cc", "buy_and_sell_stock_k_times.tsv",
      &BuyAndSellStockKTimes, DefaultComparator{}, param_names);
}
