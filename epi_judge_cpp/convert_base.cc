#include <string>
#include "test_framework/generic_test.h"
using std::string;

std::pair<int, bool> ConvertToInt(const string& num_as_string, int base) {
  bool isNegative = false;
  if (num_as_string[0] == '-') {
    isNegative = true;
  }

  auto iter = isNegative ? num_as_string.begin()+1 : num_as_string.begin();

  int currSum = 0;
  while(iter != num_as_string.end()) {
    char c = *iter;
    currSum *= base;
    currSum += std::isdigit(c) ? (c - '0') : (10 + c - 'A');
    ++iter;
  }

  //std::cout << "After converting " << num_as_string << " to base " << base << " " << currSum << std::endl;
  return std::make_pair(currSum, isNegative);
}

string ConvertToString(int num, int base) {
  if (num == 0) {
    return "0";
  }

  string retVal;
  while(num) {
    int mod = num % base;
    char c = mod >= 10 ? 'A' + (mod-10) : mod + '0';
    //std::cout << "Mod is " << mod << " appending " << c << std::endl;
    retVal.append(1, c);
    num = num/base;
  }

  std::reverse(retVal.begin(), retVal.end());
  return retVal;
}

string ConvertBase(const string& num_as_string, int b1, int b2) {
  auto convert = ConvertToInt(num_as_string, b1);

  std::string retVal = ConvertToString(convert.first, b2);
  if (convert.second) { // if isNegative is true
    retVal.insert(0, 1, '-');
  }

  return retVal;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num_as_string", "b1", "b2"};
  return GenericTestMain(args, "convert_base.cc", "convert_base.tsv",
                         &ConvertBase, DefaultComparator{}, param_names);
}
