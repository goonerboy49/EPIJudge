#include "binary_tree_node.h"
#include "test_framework/generic_test.h"

bool CheckSymmetry(const unique_ptr<BinaryTreeNode<int>>& tree1,
                   const unique_ptr<BinaryTreeNode<int>>& tree2) 
{
  if (tree1 == nullptr && tree2 == nullptr) {
    return true;
  } else if (tree1 && tree2) {
    return (tree1->data == tree2->data) && CheckSymmetry(tree1->left, tree2->right) && CheckSymmetry(tree1->right, tree2->left);
  }

  return false;
}


bool IsSymmetric(const unique_ptr<BinaryTreeNode<int>>& tree) {
  return tree == nullptr || CheckSymmetry(tree->left, tree->right);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "is_tree_symmetric.cc", "is_tree_symmetric.tsv",
                         &IsSymmetric, DefaultComparator{}, param_names);
}
