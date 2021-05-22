#include "list_node.h"
#include "test_framework/generic_test.h"
shared_ptr<ListNode<int>> EvenOddMerge(const shared_ptr<ListNode<int>>& L) {
  shared_ptr<ListNode<int>> even = make_shared<ListNode<int>>(0, nullptr);
  shared_ptr<ListNode<int>> odd = make_shared<ListNode<int>>(0, nullptr);

  std::array<shared_ptr<ListNode<int>>, 2> evenOddArray{even, odd};

  shared_ptr<ListNode<int>> curr = L;
  for (int i = 0; curr != nullptr ;i++, curr = curr->next) {
    evenOddArray[i%2]->next = curr;
    evenOddArray[i%2] = curr;
  }

  evenOddArray[0]->next = odd->next;
  evenOddArray[1]->next = nullptr;
  return even->next;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "even_odd_list_merge.cc",
                         "even_odd_list_merge.tsv", &EvenOddMerge,
                         DefaultComparator{}, param_names);
}
