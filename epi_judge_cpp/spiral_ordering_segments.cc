#include <iostream>
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
  /*int iterations = ceil(square_matrix.size() * 0.5);

  for (int offset = 0; offset < iterations; offset++) {
    TraverseMatrix(square_matrix, offset, spiralOrdering);
  }*/

  // Spiral ordering in one loop
  std::array<std::array<int, 2>, 4> dir = {{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};
  //std::cout << "111 Matrix size is " << square_matrix.size() << std::endl;
  int x = 0, y = 0;
  int currDir = 0;
  for (int i = 0; i < square_matrix.size()*square_matrix.size(); ++i) {
    spiralOrdering.emplace_back(square_matrix[x][y]);
    square_matrix[x][y] = -1;
    int next_x = x+dir[currDir][0];
    //std::cout << "Set next_x" <<std::endl;
    int next_y = y+dir[currDir][1];
    //std::cout << "222 Added x: " << x << " y: " << y << " next x: " << next_x << " next y: " << next_y << std::endl;
    if (next_x >= square_matrix.size() || next_y >= square_matrix.size() ||
        next_x < 0 || next_y < 0 || square_matrix[next_x][next_y] == -1) {
          currDir = (currDir + 1) %4;
          next_x = x + dir[currDir][0];
          next_y = y + dir[currDir][1];
          //std::cout << "333 Changed direction to " << currDir << std::endl;
        }
    x = next_x;
    y = next_y;
    //std::cout << "444 New x: " << x << " y: " << y << " next x: " << next_x << " next y: " << next_y << std::endl;
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
