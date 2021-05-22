#include "binary_tree_node.h"
#include "test_framework/generic_test.h"

/*
 * Notes 
 * 
 * - When a node returns the sum of values for all descendants remember that it WILL NOT
 * be double counted in the parent (imprint this to memory).
 * - If passing down a reference to the answer the partial_sum in the parent needs to saved in 
 * stack
 */

int SumRootToLeafHelper(BinaryTreeNode<int>* node, int partial_sum) {
  if (node == nullptr) {
    return 0;
  }

  partial_sum = partial_sum * 2 + node->data;
  if (node->left == nullptr && node->right == nullptr) {
    return partial_sum;
  }

  return SumRootToLeafHelper(node->left.get(), partial_sum) + SumRootToLeafHelper(node->right.get(), partial_sum);
}

int SumRootToLeaf(const unique_ptr<BinaryTreeNode<int>>& tree) {
  return SumRootToLeafHelper(tree.get(), 0);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "sum_root_to_leaf.cc", "sum_root_to_leaf.tsv",
                         &SumRootToLeaf, DefaultComparator{}, param_names);
}
