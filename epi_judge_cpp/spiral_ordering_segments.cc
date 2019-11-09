#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

void TraverseMatrix(const vector<vector<int>>& square_matrix, int offset, vector<int>& spiralOrdering)
{
  int sqSize = square_matrix.size();

  if (offset == sqSize-offset-1) {
    spiralOrdering.emplace_back(square_matrix[offset][offset]);
    return;
  }

  // Add n-offset-1 elems from the top row
  for (int i = offset; i < sqSize-offset-1; i++) {
    spiralOrdering.emplace_back(square_matrix[offset][i]);
  }

  // Add n-offset-1 elems from the last column
  for (int i = offset; i < sqSize-offset-1; i++) {
    spiralOrdering.emplace_back(square_matrix[i][sqSize-offset-1]);
  }

  // Add last n-offset-1 elems from the bottom row
  for (int i = sqSize-offset-1; i > offset; i--) {
    spiralOrdering.emplace_back(square_matrix[sqSize-offset-1][i]);
  }

  // Add last n-offset-1 elems from the first column
  for (int i = sqSize-offset-1; i > offset; i--) {
    spiralOrdering.emplace_back(square_matrix[i][offset]);
  }
}

vector<int> MatrixInSpiralOrder(vector<vector<int>> square_matrix) {
  vector<int> spiralOrdering;
  int iterations = ceil(square_matrix.size() * 0.5);

  for (int offset = 0; offset < iterations; offset++) {
    TraverseMatrix(square_matrix, offset, spiralOrdering);
  }

  return spiralOrdering;
}



int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"square_matrix"};
  return GenericTestMain(args, "spiral_ordering_segments.cc",
                         "spiral_ordering_segments.tsv", &MatrixInSpiralOrder,
                         DefaultComparator{}, param_names);
}
