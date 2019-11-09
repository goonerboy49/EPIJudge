#include <iostream>
#include <memory>
#include <climits>

#include "bst_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::vector;

using namespace std;

int
FindIdxGreaterThan(const vector<int>& preorder_sequence, int start, int end, int root) {
  while(start < end && preorder_sequence[start] <= root) {
    start++;
  }
  return start;
}

unique_ptr<BstNode<int>> 
RebuildBstRecurse(int& currIdx, int low, int high, const vector<int>& preorder_sequence) {
  if (currIdx == preorder_sequence.size()) {
    return nullptr;
  }

  int rootData = preorder_sequence[currIdx];

  if (rootData < low || rootData > high) {
    return nullptr;
  }

  ++currIdx;
  unique_ptr<BstNode<int>> left = RebuildBstRecurse(currIdx, low, rootData, preorder_sequence);
  unique_ptr<BstNode<int>> right = RebuildBstRecurse(currIdx, rootData, high, preorder_sequence);

  unique_ptr<BstNode<int>> root = std::make_unique<BstNode<int>>(rootData, std::move(left), std::move(right));

  return root;
}

unique_ptr<BstNode<int>> RebuildBSTFromPreorder(
    const vector<int>& preorder_sequence) {
  
  int currIdx = 0;
  return RebuildBstRecurse(currIdx, INT_MIN, INT_MAX, preorder_sequence);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"preorder_sequence"};
  return GenericTestMain(args, "bst_from_preorder.cc", "bst_from_preorder.tsv",
                         &RebuildBSTFromPreorder, DefaultComparator{},
                         param_names);
}
