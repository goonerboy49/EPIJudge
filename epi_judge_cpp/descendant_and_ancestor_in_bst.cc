#include <memory>
#include "bst_node.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::unique_ptr;

bool SearchPath(const unique_ptr<BstNode<int>>& from, const unique_ptr<BstNode<int>>& to) {
  BstNode<int> *iter = from.get();

  while(iter && iter != to.get()) {
    iter = iter->data > to->data ? iter->left.get() : iter->right.get();
  }

  return iter == to.get();
}

bool PairIncludesAncestorAndDescendantOfM(
    const unique_ptr<BstNode<int>>& possible_anc_or_desc_0,
    const unique_ptr<BstNode<int>>& possible_anc_or_desc_1,
    const unique_ptr<BstNode<int>>& middle) {
  BstNode<int> *from0 = possible_anc_or_desc_0.get();
  BstNode<int> *from1 = possible_anc_or_desc_1.get();

  int middleData = middle->data;
  while(from0 != possible_anc_or_desc_1.get() && from0 != middle.get() && from1 != middle.get() && from1 != possible_anc_or_desc_0.get()) {
    if (from0) {
      from0 = from0->data > middleData ? from0->left.get() : from0->right.get();
    }

    if (from1) {
      from1 = from1->data > middleData ? from1->left.get() : from1->right.get();
    }
  }

  if ((from0 != middle.get() && from1 != middle.get()) || from0 == possible_anc_or_desc_1.get() || from1 == possible_anc_or_desc_0.get()) {
    return false;  
  }

  return SearchPath(middle, from0 == middle.get() ? possible_anc_or_desc_1 : possible_anc_or_desc_0);
}
bool PairIncludesAncestorAndDescendantOfMWrapper(
    TimedExecutor& executor, const unique_ptr<BstNode<int>>& tree,
    int possible_anc_or_desc_0, int possible_anc_or_desc_1, int middle) {
  auto& candidate0 = MustFindNode(tree, possible_anc_or_desc_0);
  auto& candidate1 = MustFindNode(tree, possible_anc_or_desc_1);
  auto& middle_node = MustFindNode(tree, middle);
  return executor.Run([&] {
    return PairIncludesAncestorAndDescendantOfM(candidate0, candidate1,
                                                middle_node);
  });
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "tree",
                                       "possible_anc_or_desc_0",
                                       "possible_anc_or_desc_1", "middle"};
  return GenericTestMain(args, "descendant_and_ancestor_in_bst.cc",
                         "descendant_and_ancestor_in_bst.tsv",
                         &PairIncludesAncestorAndDescendantOfMWrapper,
                         DefaultComparator{}, param_names);
}
