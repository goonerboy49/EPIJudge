#include <unordered_map>
#include <vector>
#include "binary_tree_node.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
using std::vector;

unique_ptr<BinaryTreeNode<int>> CreateTree(int preorderStart, int preorderEnd,
    int inorderStart, int inorderEnd, const std::unordered_map<int, int>& inorderIdxMap, 
    const vector<int>& preorder, const vector<int>& inorder) {
  if (preorderEnd <= preorderStart || inorderEnd <= inorderStart) {
    return nullptr;
  }

  int inorderIdx = inorderIdxMap.at(preorder[preorderStart]);
  size_t leftSubtreeSize = inorderIdx - inorderStart;
  return std::make_unique<BinaryTreeNode<int>>(inorder[inorderIdx], 
    CreateTree(preorderStart+1, preorderStart+leftSubtreeSize+1, 
              inorderStart, inorderIdx, inorderIdxMap, preorder, inorder),
    CreateTree(preorderStart+leftSubtreeSize + 1, preorderEnd, 
               inorderIdx+1, inorderEnd, inorderIdxMap, preorder, inorder));
}

unique_ptr<BinaryTreeNode<int>> BinaryTreeFromPreorderInorder(
    const vector<int>& preorder, const vector<int>& inorder) {
  std::unordered_map<int, int> inorderIdxMap;
  for (int i = 0; i < inorder.size(); i++) {
    inorderIdxMap[inorder[i]] = i;
  }

  return CreateTree(0, preorder.size(), 0, inorder.size(), inorderIdxMap, preorder, inorder);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"preorder", "inorder"};
  return GenericTestMain(
      args, "tree_from_preorder_inorder.cc", "tree_from_preorder_inorder.tsv",
      &BinaryTreeFromPreorderInorder, DefaultComparator{}, param_names);
}
