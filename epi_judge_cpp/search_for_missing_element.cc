#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;

struct DuplicateAndMissing {
  int duplicate, missing;
};

DuplicateAndMissing FindDuplicateMissing(const vector<int>& A) {
  // XOR all elements in the array with elements from 0 to A.size() - 1 gives the m ^ d value
  int missing_xor_dupe = 0;
  int i = 0;
  for (int a : A) {
    missing_xor_dupe = missing_xor_dupe ^ a;
    missing_xor_dupe = missing_xor_dupe ^ i;
    ++i;
  }

  // Find all elements whose least significant bit is set
  int differ_bit = missing_xor_dupe & ~(missing_xor_dupe - 1);
  int miss_or_dup = 0;
  for (int i = 0; i < A.size(); i++) {
    if (i & differ_bit) {
      miss_or_dup = miss_or_dup ^ i;
    }

    if (A[i] & differ_bit) {
      miss_or_dup = miss_or_dup ^ A[i];
    }
  }

  if (std::find(A.begin(), A.end(), miss_or_dup) != A.end()) {
    return {miss_or_dup, miss_or_dup ^ missing_xor_dupe};
  } 
  return {miss_or_dup ^ missing_xor_dupe, miss_or_dup};
}
template <>
struct SerializationTraits<DuplicateAndMissing>
    : UserSerTraits<DuplicateAndMissing, int, int> {};

bool operator==(const DuplicateAndMissing& lhs,
                const DuplicateAndMissing& rhs) {
  return std::tie(lhs.duplicate, lhs.missing) ==
         std::tie(rhs.duplicate, rhs.missing);
}

std::ostream& operator<<(std::ostream& out, const DuplicateAndMissing& x) {
  return out << "duplicate: " << x.duplicate << ", missing: " << x.missing;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "search_for_missing_element.cc", "find_missing_and_duplicate.tsv",
      &FindDuplicateMissing, DefaultComparator{}, param_names);
}
