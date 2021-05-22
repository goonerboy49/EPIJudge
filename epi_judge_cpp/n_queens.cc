#include <algorithm>
#include <iterator>
#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

void isValid(int row, int n, vector<int>& prevQPositions, vector<vector<int>>& ans) {
  if (row == n) {
    ans.emplace_back(prevQPositions);
    return;
  }

  // Try placing the queen in each col of this row and
  // bail if it clashes with a queen placement in one 
  // of the previous rows.
  for (int col = 0; col < n; col++) {
    bool placementFound = true;
    for (int prevRow = 0; prevRow < row; prevRow++) {
      int prevCol = prevQPositions[prevRow];
      if (prevCol == col || (prevRow+prevCol == row + col || prevRow-prevCol == row-col)) {
        placementFound = false;
        break;
      }
    }

    if(placementFound) {
      prevQPositions[row] = col;
      isValid(row+1, n, prevQPositions, ans);
      prevQPositions[row] = -1;
    }
  }
}


vector<vector<int>> NQueens(int n) {
  std::vector<int> qPositions(n, -1);
  vector<vector<int>> ans;
  isValid(0, n, qPositions, ans);
  return ans;
}
bool Comp(vector<vector<int>>& a, vector<vector<int>>& b) {
  std::sort(std::begin(a), std::end(a));
  std::sort(std::begin(b), std::end(b));
  return a == b;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "n_queens.cc", "n_queens.tsv", &NQueens, &Comp,
                         param_names);
}
