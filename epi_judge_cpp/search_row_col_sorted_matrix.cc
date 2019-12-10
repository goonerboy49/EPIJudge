#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

bool MatrixSearch(const vector<vector<int>>& A, int x) {
  int m = A.size();
  if (!m) return false;

  int n = A[0].size();

  int currRow = 0;
  int currCol = n-1;
  while(currRow >= 0 && currRow < m && currCol >=0 && currCol < n) {
    int nextElem = A[currRow][currCol];

    if (x == nextElem) {
      return true;
    } else if (x > nextElem) {
      ++currRow;
    } else {
      --currCol;
    }
  }
  return false;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "x"};
  return GenericTestMain(args, "search_row_col_sorted_matrix.cc",
                         "search_row_col_sorted_matrix.tsv", &MatrixSearch,
                         DefaultComparator{}, param_names);
}
