#include <memory>
#include <vector>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::vector;

struct Element{
  BinaryTreeNode<int>* node;
  bool isLeftTraversed;
};

vector<int> InorderTraversal(const unique_ptr<BinaryTreeNode<int>>& tree) {
  std::stack<Element> inorderStack;
  vector<int> result;
  if (tree == nullptr) {
    return result;
  }

  inorderStack.push({tree.get(), false});

  while (!inorderStack.empty())
  {
    auto topElement = inorderStack.top();
    inorderStack.pop();

    if (topElement.isLeftTraversed) {
      result.emplace_back(topElement.node->data);
    } else {
      if (topElement.node->right) {
        inorderStack.push({topElement.node->right.get(), false});
      }
      inorderStack.push({topElement.node, true});
      if (topElement.node->left) {  
        inorderStack.push({topElement.node->left.get(), false});
      }
    }
  }
  return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_inorder.cc", "tree_inorder.tsv",
                         &InorderTraversal, DefaultComparator{}, param_names);
}
