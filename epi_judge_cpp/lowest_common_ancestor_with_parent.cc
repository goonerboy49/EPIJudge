#include "binary_tree_with_parent_prototype.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"

int GetDepth(BinaryTreeNode<int> *node) {
  int depth = 0;
  while(node) {
    node = node->parent;
    ++depth;
  }
  return depth;
}

BinaryTreeNode<int>* LCA(const unique_ptr<BinaryTreeNode<int>>& node0,
                         const unique_ptr<BinaryTreeNode<int>>& node1) {
  int node0Depth = GetDepth(node0.get());
  int node1Depth = GetDepth(node1.get());

  BinaryTreeNode<int> *node0Ptr = node0.get();
  BinaryTreeNode<int> *node1Ptr = node1.get();
  if (node1Depth > node0Depth) {
    std::swap(node0Ptr, node1Ptr);
  }

  int diff = abs(node0Depth - node1Depth);
  while(diff) {
    node0Ptr = node0Ptr->parent;
    --diff;
  }

  while(node0Ptr != node1Ptr) {
    node0Ptr = node0Ptr->parent;
    node1Ptr = node1Ptr->parent;
  }

  return node0Ptr;
}
int LcaWrapper(TimedExecutor& executor,
               const unique_ptr<BinaryTreeNode<int>>& tree, int key0,
               int key1) {
  const unique_ptr<BinaryTreeNode<int>>& node0 = MustFindNode(tree, key0);
  const unique_ptr<BinaryTreeNode<int>>& node1 = MustFindNode(tree, key1);

  auto result = executor.Run([&] { return LCA(node0, node1); });

  if (!result) {
    throw TestFailure("Result can not be nullptr");
  }
  return result->data;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "tree", "key0", "key1"};
  return GenericTestMain(args, "lowest_common_ancestor_with_parent.cc",
                         "lowest_common_ancestor.tsv", &LcaWrapper,
                         DefaultComparator{}, param_names);
}
