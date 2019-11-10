#include <set>
#include <stdexcept>
#include "list_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"

using ListNodePtr = shared_ptr<ListNode<int>>;

shared_ptr<ListNode<int>> HasCycle(const shared_ptr<ListNode<int>>& head) {
  shared_ptr<ListNode<int>> slow = head, fast = head;

  while(slow && fast) {
    slow = slow->next;
    fast = fast->next;
    if (fast) {
      fast = fast->next;
    }

    if (slow == fast) {
      break;
    }
  }

  if (fast == nullptr) {
    return nullptr;
  }

  shared_ptr<ListNode<int>> curr = head;
  while(curr != fast) {
    curr = curr->next;
    fast = fast->next;
  }

  return curr;
}

int GetLength(ListNodePtr l) {
  int len = 0;
  while(l) {
    l = l->next;
    ++len;
  }
  return len;
}

void AdvanceListPtr(ListNodePtr& l, int k) {
  while(k) {
    l = l->next;
    --k;
  }
}

shared_ptr<ListNode<int>> OverlappingNoCycleLists(
    shared_ptr<ListNode<int>> l0, shared_ptr<ListNode<int>> l1) {
  int l0Len = GetLength(l0);
  int l1Len = GetLength(l1);
  //std::cout << "List0 length " << l0Len << " List1 length " << l1Len << std::endl;

  int diff = abs(l0Len - l1Len);
  AdvanceListPtr(l0Len > l1Len ? l0 : l1, diff);

  while(l0 && l1) {
    if (l0 == l1) {
      return l0;
    }
    l0 = l0->next;
    l1 = l1->next;
  }
  
  return nullptr;
}

int Distance(ListNodePtr a, ListNodePtr b) {
  int dist = 0;
  while(a != b) {
    a = a->next;
    dist++;
  }
  return dist;
}

shared_ptr<ListNode<int>> OverlappingLists(shared_ptr<ListNode<int>> l0,
                                           shared_ptr<ListNode<int>> l1) {
  ListNodePtr root0 = HasCycle(l0);
  ListNodePtr root1 = HasCycle(l1);

  if (!root0 && !root1) {
    return OverlappingNoCycleLists(l0, l1);
  }

  if ((root0 && !root1) || (!root0 && root1)) {
    return nullptr;
  }

  ListNodePtr temp = root0;
  do {
    temp = temp->next;
  } while(temp != root1 && temp != root0);

  // Both lists have cycles but they do not overlap
  if (temp != root1) {
    return nullptr;
  }

  int l0CycleOffset = Distance(l0, root0);
  int l1CycleOffset = Distance(l1, root1);

  int diff = abs(l0CycleOffset - l1CycleOffset);

  l0CycleOffset > l1CycleOffset ? AdvanceListPtr(l0, diff) : AdvanceListPtr(l1, diff);

  while (l0 != l1 && l0 != root0 && l1 != root1) {
    l0 = l0->next;
    l1 = l1->next;
  }

  return l0 == l1 ? l0 : root1;
}
void OverlappingListsWrapper(TimedExecutor& executor,
                             shared_ptr<ListNode<int>> l0,
                             shared_ptr<ListNode<int>> l1,
                             shared_ptr<ListNode<int>> common, int cycle0,
                             int cycle1) {
  if (common) {
    if (!l0) {
      l0 = common;
    } else {
      auto it = l0;
      while (it->next) {
        it = it->next;
      }
      it->next = common;
    }

    if (!l1) {
      l1 = common;
    } else {
      auto it = l1;
      while (it->next) {
        it = it->next;
      }
      it->next = common;
    }
  }

  if (cycle0 != -1 && l0) {
    auto last = l0;
    while (last->next) {
      last = last->next;
    }
    auto it = l0;
    while (cycle0-- > 0) {
      if (!it) {
        throw std::runtime_error("Invalid input data");
      }
      it = it->next;
    }
    last->next = it;
  }

  if (cycle1 != -1 && l1) {
    auto last = l1;
    while (last->next) {
      last = last->next;
    }
    auto it = l1;
    while (cycle1-- > 0) {
      if (!it) {
        throw std::runtime_error("Invalid input data");
      }
      it = it->next;
    }
    last->next = it;
  }

  std::set<shared_ptr<ListNode<int>>> common_nodes;
  auto it = common;
  while (it && common_nodes.count(it) == 0) {
    common_nodes.insert(it);
    it = it->next;
  }

  auto result = executor.Run([&] { return OverlappingLists(l0, l1); });

  if (!((common_nodes.empty() && result == nullptr) ||
        common_nodes.count(result) > 0)) {
    throw TestFailure("Invalid result");
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "l0",     "l1",
                                       "common",   "cycle0", "cycle1"};
  return GenericTestMain(args, "do_lists_overlap.cc", "do_lists_overlap.tsv",
                         &OverlappingListsWrapper, DefaultComparator{},
                         param_names);
}
