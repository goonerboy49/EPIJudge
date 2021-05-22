#include <memory>
#include "list_node.h"
#include "test_framework/generic_test.h"
using std::shared_ptr;

shared_ptr<ListNode<int>> RemoveDuplicates(const shared_ptr<ListNode<int>>& L) {
  shared_ptr<ListNode<int>> curr = L;

  while (curr != nullptr) 
  {
    int val = curr->data;
    shared_ptr<ListNode<int>> temp = curr->next;
    while(temp != nullptr && curr->data == temp->data) {
      temp = temp->next;
    }
    curr->next = temp;
    curr = curr->next;
  }
  
  return L;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "remove_duplicates_from_sorted_list.cc",
                         "remove_duplicates_from_sorted_list.tsv",
                         &RemoveDuplicates, DefaultComparator{}, param_names);
}
