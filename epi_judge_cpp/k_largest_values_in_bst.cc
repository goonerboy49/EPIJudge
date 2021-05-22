#include <memory>
#include <vector>
#include "bst_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::vector;

/**
 * Notes -
 * Do a reverse inorder by visiting the right most node first.
 * Check the size before adding to result very important.
 */ 

void LargestInBstRecurse(BstNode<int> *node, vector<int>& ans, int k) {
  if (node != nullptr && ans.size() < k) {
    LargestInBstRecurse(node->right.get(), ans, k);
    if (ans.size() < k) {
      ans.emplace_back(node->data);
      LargestInBstRecurse(node->left.get(), ans, k);
    }
  }
}

vector<int> FindKLargestInBST(const unique_ptr<BstNode<int>>& tree, int k) {
  vector<int> ans;
  LargestInBstRecurse(tree.get(),ans, k);

  return ans;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree", "k"};
  return GenericTestMain(args, "k_largest_values_in_bst.cc",
                         "k_largest_values_in_bst.tsv", &FindKLargestInBST,
                         &UnorderedComparator<std::vector<int>>, param_names);
}
