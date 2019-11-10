#include <memory>
#include "list_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::shared_ptr;

using ListNodePtr = shared_ptr<ListNode<int>>;

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
void OverlappingNoCycleListsWrapper(TimedExecutor& executor,
                                    shared_ptr<ListNode<int>> l0,
                                    shared_ptr<ListNode<int>> l1,
                                    shared_ptr<ListNode<int>> common) {
  if (common) {
    if (l0) {
      auto i = l0;
      while (i->next) {
        i = i->next;
      }
      i->next = common;
    } else {
      l0 = common;
    }

    if (l1) {
      auto i = l1;
      while (i->next) {
        i = i->next;
      }
      i->next = common;
    } else {
      l1 = common;
    }
  }

  auto result = executor.Run([&] { return OverlappingNoCycleLists(l0, l1); });

  if (result != common) {
    throw TestFailure("Invalid result");
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "l0", "l1", "common"};
  return GenericTestMain(
      args, "do_terminated_lists_overlap.cc", "do_terminated_lists_overlap.tsv",
      &OverlappingNoCycleListsWrapper, DefaultComparator{}, param_names);
}
