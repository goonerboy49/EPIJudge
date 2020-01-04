#include <iostream>
#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int CalculateLargestRectangle(const vector<int>& heights) {
  std::stack<int> indices;
  int ans = 0;

  for (int i = 0; i <= heights.size(); i++) {
    while(!indices.empty() &&
          (i < heights.size() ? (heights[indices.top()] >= heights[i]): true)) {
      int height = heights[indices.top()];
      indices.pop();

      int width = !indices.empty() ? (i - indices.top() -1) : i;
      ans = std::max(ans, (width * height));
    }
    indices.push(i);
  }

  return ans;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"heights"};
  return GenericTestMain(args, "largest_rectangle_under_skyline.cc",
                         "largest_rectangle_under_skyline.tsv",
                         &CalculateLargestRectangle, DefaultComparator{},
                         param_names);
}
