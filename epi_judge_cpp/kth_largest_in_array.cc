#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

// The numbering starts from one, i.e., if A = [3, 1, -1, 2] then
// FindKthLargest(1, A) returns 3, FindKthLargest(2, A) returns 2,
// FindKthLargest(3, A) returns 1, and FindKthLargest(4, A) returns -1.
int FindPlacement(vector<int>& A, int left, int right, int pivotIdx) {
  int pivotElem = A[pivotIdx];
  int actualPivotIdx = left;
  std::swap(A[pivotIdx], A[right]);

  for(int i = left; i < right; i++) {
    if (A[i] < pivotElem) {
      std::swap(A[actualPivotIdx], A[i]);
      ++actualPivotIdx;
    }
  }

  std::swap(A[right], A[actualPivotIdx]);
  return actualPivotIdx;
}


int Partition(vector<int>& A, int left, int right, int kthSmallest) {
  if (left == right) {
    return A[left];
  }

  int pivotIdx = left + rand() % (right - left);

  pivotIdx = FindPlacement(A, left, right, pivotIdx);

  if (pivotIdx == kthSmallest) {
    return A[kthSmallest];
  } else if (pivotIdx < kthSmallest) {
    return Partition(A, pivotIdx+1, right, kthSmallest);
  } else {
    return Partition(A, left, pivotIdx-1, kthSmallest);
  }
}

int FindKthLargest(int k, vector<int>* A_ptr) {
  if (!A_ptr->size()) {
    return 0;
  }
  return Partition(*A_ptr, 0, A_ptr->size()-1, A_ptr->size()-k);
}
int FindKthLargestWrapper(int k, vector<int>& A) {
  return FindKthLargest(k, &A);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"k", "A"};
  return GenericTestMain(args, "kth_largest_in_array.cc",
                         "kth_largest_in_array.tsv", &FindKthLargestWrapper,
                         DefaultComparator{}, param_names);
}
