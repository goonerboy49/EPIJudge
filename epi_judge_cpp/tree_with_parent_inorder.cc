#include <vector>
#include "binary_tree_with_parent_prototype.h"
#include "test_framework/generic_test.h"
using std::vector;

vector<int> InorderTraversal(const unique_ptr<BinaryTreeNode<int>>& tree) {
  BinaryTreeNode<int> *prev = nullptr;
  BinaryTreeNode<int> *curr = tree.get();
  vector<int> result;

  while(curr) {
    BinaryTreeNode<int> *next;
    // If we came down to curr from its parent
    if (curr->parent == prev) {
      if (curr->left != nullptr) {
        next = curr->left.get();
      } else {
        // Visit current node and go right
        result.emplace_back(curr->data);
        // If there is no right subtree then go up a level
        next = curr->right != nullptr ? curr->right.get() : curr->parent;
      }
    } else if (prev == curr->left.get()) {// If we go up check if it is from left subtree
      result.emplace_back(curr->data);
      // If there is no right subtree then go up a level
      next = curr->right != nullptr ? curr->right.get() : curr->parent;
    } else {
      next = curr->parent;
    }
    
    prev = curr;
    curr = next;
  }
  return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_with_parent_inorder.cc",
                         "tree_with_parent_inorder.tsv", &InorderTraversal,
                         DefaultComparator{}, param_names);
}
