#include "binary_tree_with_parent_prototype.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"

BinaryTreeNode<int>* FindSuccessor(
    const unique_ptr<BinaryTreeNode<int>>& node) {
  // If the node has a right subtree then inorder successor is the leftmost
  // node in right subtree

  BinaryTreeNode<int>* iter = node.get();

  if (iter->right != nullptr) {
    // Traverse leftmost node in the right subtree.
    iter = iter->right.get();
    while (iter->left.get() != nullptr) {
      iter == iter->left.get();
    }

    return iter;
  }

  // If there is no right subtree travel up this branch until a node that
  // is the left child is found along the path traced using parent pointer.
  // REMEMBER To check iter->parent and not iter->parent. because this 
  // could traverse up to the root.
  while(iter->parent != nullptr && iter->parent->right.get() == iter) {
    iter = iter->parent;
  }

  return iter->parent;
}
int FindSuccessorWrapper(const unique_ptr<BinaryTreeNode<int>>& tree,
                         int node_idx) {
  auto result = FindSuccessor(MustFindNode(tree, node_idx));
  return result ? result->data : -1;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree", "node_idx"};
  return GenericTestMain(args, "successor_in_tree.cc", "successor_in_tree.tsv",
                         &FindSuccessorWrapper, DefaultComparator{},
                         param_names);
}
