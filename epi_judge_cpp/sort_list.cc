#include <memory>
#include "list_node.h"
#include "test_framework/generic_test.h"

shared_ptr<ListNode<int>> StableSortList(shared_ptr<ListNode<int>> L) {
  auto dummy = std::make_shared<ListNode<int>>(ListNode<int>{0, L});
  auto iter = L;

  while(iter && iter->next) {
    if (iter->data > iter->next->data) {
      auto pre = dummy;
      auto target = iter->next;
      while(pre->next->data < target->data) {
        pre = pre->next;
      }
      auto temp = pre->next;
      pre->next = target;
      iter->next = target->next;
      target->next = temp;
    } else {
      iter = iter->next;
    }
  }
  return dummy->next;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "sort_list.cc", "sort_list.tsv", &StableSortList,
                         DefaultComparator{}, param_names);
}
