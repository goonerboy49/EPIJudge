#include "test_framework/generic_test.h"
long long Reverse(int x) {
  // TODO - you fill in here.
  long long result = 0;
  while(x) {
    int lastDigit = x % 10;
    result = result*10+lastDigit;
    x = x/10;
  }

  return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "reverse_digits.cc", "reverse_digits.tsv",
                         &Reverse, DefaultComparator{}, param_names);
}
