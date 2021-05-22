#include "list_node.h"
#include "test_framework/generic_test.h"

/**
 * Split the list into 2 by using the fast and slow pointer approach.
 * Then reverse the second half of the list and check node by node 
 * the first half and second half
 * 
 * Important when reversing the list to check for boundary conditions 
 * such as if the input list has just one element the second half contains
 * no elements.!!
 */
shared_ptr<ListNode<int>> ReverseList(shared_ptr<ListNode<int>> head) {
  if (head == nullptr) {
    return nullptr;
  }
  shared_ptr<ListNode<int>> prev = make_shared<ListNode<int>>(0, head);
  shared_ptr<ListNode<int>> curr = head;
  while(curr->next != nullptr) {
    shared_ptr<ListNode<int>> temp = curr->next;
    curr->next = temp->next;
    temp->next = prev->next;
    prev->next = temp;
  }

  return prev->next;
}

bool IsLinkedListAPalindrome(shared_ptr<ListNode<int>> L) {
  if (L == nullptr) {
    return true;
  }
  // Find the middle point of the list
  shared_ptr<ListNode<int>> slow = L;
  shared_ptr<ListNode<int>> fast = L;

  while (fast != nullptr) {
    slow = slow->next;
    fast = fast->next;
    if (fast) {
      fast = fast->next;
    }
  }

  shared_ptr<ListNode<int>> second = ReverseList(slow);
  shared_ptr<ListNode<int>> first = L;
  while(first != nullptr && second != nullptr) {
    if (first->data != second->data) {
      return false;
    }
    first = first->next;
    second = second->next;
  }

  return true;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "is_list_palindromic.cc",
                         "is_list_palindromic.tsv", &IsLinkedListAPalindrome,
                         DefaultComparator{}, param_names);
}
