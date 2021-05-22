#include "list_node.h"
#include "test_framework/generic_test.h"

shared_ptr<ListNode<int>> ReverseSublist(shared_ptr<ListNode<int>> L, int start,
                                         int finish) {
  // Traverse the list such that currHead is the start;
  int s = 1;
  shared_ptr<ListNode<int>> dummy = std::make_shared<ListNode<int>>(ListNode<int>(0, L));
  shared_ptr<ListNode<int>> subListHead = dummy;
  while(s < start) {
    subListHead = subListHead->next;
    ++s;
  }

  shared_ptr<ListNode<int>> prev = subListHead;
  shared_ptr<ListNode<int>> curr = subListHead->next;

  /**
   * Here the objective is such that for each new node encountered(temp)
   * it is moved to the start position of the sublist, repeatedly doing 
   * this for all nodes in the sublist will reverse the sublist between
   * start and finish.
   */
  while(s < finish && curr) {
    shared_ptr<ListNode<int>> temp = curr->next;
    curr->next = temp->next;
    temp->next = prev->next;
    prev->next = temp;
    s++;
  }

  return dummy->next;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L", "start", "finish"};
  return GenericTestMain(args, "reverse_sublist.cc", "reverse_sublist.tsv",
                         &ReverseSublist, DefaultComparator{}, param_names);
}
