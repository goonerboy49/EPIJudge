#include <iostream>
#include <string>
#include <vector>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::vector;

unique_ptr<BinaryTreeNode<int>> ReconstructPreorderHelper(const vector<int*>& preorder, int *currIdx){
  int *currKey = preorder[*currIdx];
  int& currIdxRef = *currIdx;
  currIdxRef++;

  if (currKey == nullptr) {
    return nullptr;
  }

  //std::cout << "Trying index " << currIdxRef << std::endl;
  unique_ptr<BinaryTreeNode<int>> currNode = std::make_unique<BinaryTreeNode<int>>(*currKey);
  currNode->left = std::move(ReconstructPreorderHelper(preorder, currIdx));
  currNode->right = std::move(ReconstructPreorderHelper(preorder, currIdx));
  return currNode;
}


unique_ptr<BinaryTreeNode<int>> ReconstructPreorder(
    const vector<int*>& preorder) {
    int currIdx = 0;
    return ReconstructPreorderHelper(preorder, &currIdx);
}
unique_ptr<BinaryTreeNode<int>> ReconstructPreorderWrapper(
    TimedExecutor& executor, const vector<string>& preorder) {
  vector<int> values;
  vector<int*> ptrs;
  values.reserve(preorder.size());
  for (auto& s : preorder) {
    if (s == "null") {
      ptrs.push_back(nullptr);
    } else {
      int i = std::stoi(s);
      values.push_back(i);
      ptrs.push_back(&values.back());
    }
  }

  return executor.Run([&] { return ReconstructPreorder(ptrs); });
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "preorder"};
  return GenericTestMain(args, "tree_from_preorder_with_null.cc",
                         "tree_from_preorder_with_null.tsv",
                         &ReconstructPreorderWrapper, DefaultComparator{},
                         param_names);
}
