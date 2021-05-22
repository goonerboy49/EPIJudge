#include <algorithm>
#include <cmath>
#include <iterator>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::begin;
using std::end;
using std::vector;

/**
 * Notes -
 * - If the current square is already filled advance to the next square and return the
 * backtracking result starting from there
 * - Check if the val is valid before starting a backtracking node from there
 */ 

bool CheckVal(int r, int c, int val, const vector<vector<int>>& assignment) {
  // Check row constraints
  vector<int> row = assignment[r];
  for (int i = 1; i <= row.size(); i++) {
    if (row[i-1] == val) {
      return false;
    }
  }

  // Check col constraints
  for (int i = 1; i <= 9; i++) {
    if (assignment[i-1][c] == val) {
      return false;
    }
  }

  int const regionSize = sqrt(assignment[0].size());
  int rowRegion = r / regionSize;
  int colRegion = c / regionSize;

  for (int i = rowRegion*regionSize; i < (rowRegion+1)*regionSize; i++) {
    for (int j = colRegion*regionSize; j <(colRegion+1)*regionSize; j++) {
      if (assignment[i][j] == val) {
        return false;
      }
    }
  }

  return true;
}

bool SudokuRecurse(int r, int c, vector<vector<int>>* partial_assignment) {
  if (c == (*partial_assignment)[r].size()) {
    c = 0; // Start a new row, reset col to 0
    if (++r == (*partial_assignment).size()) {
      return true;
    }
  }

  if ((*partial_assignment)[r][c] != 0) {
    return SudokuRecurse(r, c+1, partial_assignment);
  }

  for (int i = 1; i <=(*partial_assignment).size(); i++) {
    if (CheckVal(r, c, i, *partial_assignment)) {
      (*partial_assignment)[r][c] = i;
      if (SudokuRecurse(r, c+1, partial_assignment)) {
        return true;
      }
    }
  }

  (*partial_assignment)[r][c] = 0;
  return false;
}

bool SolveSudoku(vector<vector<int>>* partial_assignment) {
  return SudokuRecurse(0, 0, partial_assignment);
}
vector<int> GatherColumn(const vector<vector<int>>& data, size_t i) {
  vector<int> result;
  for (auto& row : data) {
    result.push_back(row[i]);
  }
  return result;
}

vector<int> GatherSquareBlock(const vector<vector<int>>& data,
                              size_t block_size, size_t n) {
  vector<int> result;
  size_t block_x = n % block_size;
  size_t block_y = n / block_size;
  for (size_t i = block_x * block_size; i < (block_x + 1) * block_size; i++) {
    for (size_t j = block_y * block_size; j < (block_y + 1) * block_size; j++) {
      result.push_back(data[i][j]);
    }
  }

  return result;
}

void AssertUniqueSeq(const vector<int>& seq) {
  vector<bool> seen(seq.size(), false);
  for (auto& x : seq) {
    if (x == 0) {
      throw TestFailure("Cell left uninitialized");
    }
    if (x < 0 || x > seq.size()) {
      throw TestFailure("Cell value out of range");
    }
    if (seen[x - 1]) {
      throw TestFailure("Duplicate value in section");
    }
    seen[x - 1] = true;
  }
}

void SolveSudokuWrapper(TimedExecutor& executor,
                        const vector<vector<int>>& partial_assignment) {
  vector<vector<int>> solved = partial_assignment;

  executor.Run([&] { SolveSudoku(&solved); });

  if (!std::equal(begin(partial_assignment), end(partial_assignment),
                  begin(solved), end(solved), [](auto br, auto cr) {
                    return std::equal(begin(br), end(br), begin(cr), end(cr),
                                      [](int bcell, int ccell) {
                                        return bcell == 0 || bcell == ccell;
                                      });
                  }))
    throw TestFailure("Initial cell assignment has been changed");

  auto block_size = static_cast<size_t>(sqrt(solved.size()));

  for (size_t i = 0; i < solved.size(); i++) {
    AssertUniqueSeq(solved[i]);
    AssertUniqueSeq(GatherColumn(solved, i));
    AssertUniqueSeq(GatherSquareBlock(solved, block_size, i));
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "partial_assignment"};
  return GenericTestMain(args, "sudoku_solve.cc", "sudoku_solve.tsv",
                         &SolveSudokuWrapper, DefaultComparator{}, param_names);
}
