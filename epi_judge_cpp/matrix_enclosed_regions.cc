#include <stdexcept>
#include <string>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::vector;

void PaintBoundaryWhite(int x, int y, vector<vector<char>>& board) {
  if (board[x][y] != 'W') {
    return;
  }
  std::queue<std::pair<int, int>> q;
  q.emplace(x, y);
  vector<int> xInc = {1, 0, -1, 0};
  vector<int> yInc = {0, 1, 0, -1};

  while(!q.empty()) {
    auto front = q.front();
    q.pop();
    board[front.first][front.second] = 'T';
    for (int i = 0; i < 4; i++) {
      int nextX = front.first + xInc[i];
      int nextY = front.second + yInc[i];    
      if (nextX >= 0 && nextX < board.size() && nextY >=0 && nextY < board[0].size() && board[nextX][nextY] == 'W') {
        q.emplace(nextX, nextY);
      }
    }
  }
}

void FillSurroundedRegions(vector<vector<char>>* board_ptr) {
  // Paint first and last row
  for (int i = 0; i < board_ptr->front().size();i++) {
    PaintBoundaryWhite(0, i, *board_ptr);
    PaintBoundaryWhite(board_ptr->size()-1, i, *board_ptr);
  }

  // Paint first and last column
  for (int j = 0; j < board_ptr->size(); j++) {
    PaintBoundaryWhite(j, 0, *board_ptr);
    PaintBoundaryWhite(j, board_ptr->front().size()-1, *board_ptr);
  }

  vector<vector<char>>& board = *board_ptr;
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board.front().size(); j++) {
      board[i][j] = board[i][j] == 'T' ? 'W' : 'B';
    }
  }

  return;
}
vector<vector<string>> FillSurroundedRegionsWrapper(
    TimedExecutor& executor, vector<vector<string>> board) {
  vector<vector<char>> char_vector;
  char_vector.resize(board.size());
  for (int i = 0; i < board.size(); i++) {
    for (const string& s : board[i]) {
      if (s.size() != 1) {
        throw std::runtime_error("String size is not 1");
      }
      char_vector[i].push_back(s[0]);
    }
  }

  executor.Run([&] { FillSurroundedRegions(&char_vector); });

  board.clear();
  board.resize(char_vector.size(), {});
  for (int i = 0; i < board.size(); i++) {
    for (char c : char_vector[i]) {
      board[i].emplace_back(1, c);
    }
  }

  return board;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "board"};
  return GenericTestMain(
      args, "matrix_enclosed_regions.cc", "matrix_enclosed_regions.tsv",
      &FillSurroundedRegionsWrapper, DefaultComparator{}, param_names);
}
