#include "binary_tree_node.h"
#include "test_framework/generic_test.h"

struct Balanced {
  Balanced(bool b, int h) : isBalanced(b), height(h) {}
  bool isBalanced;
  int height;
};

Balanced CheckBalanced(const unique_ptr<BinaryTreeNode<int>>& node) {
  if (node == nullptr) {
    return Balanced(true, -1);
  }

  auto leftBalanced = CheckBalanced(node->left);
  if (!leftBalanced.isBalanced) {
    return Balanced(false, leftBalanced.height+1);
  }

  auto rightBalanced = CheckBalanced(node->right);
  if (!rightBalanced.isBalanced) {
    return Balanced(false, rightBalanced.height+1);
  }

  return Balanced(abs(leftBalanced.height - rightBalanced.height) <= 1, std::max(leftBalanced.height, rightBalanced.height)+1);
}

bool IsBalanced(const unique_ptr<BinaryTreeNode<int>>& tree) {
  return CheckBalanced(tree).isBalanced;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "is_tree_balanced.cc", "is_tree_balanced.tsv",
                         &IsBalanced, DefaultComparator{}, param_names);
}
