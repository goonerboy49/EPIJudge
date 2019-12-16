#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

bool IsPalindrome(const std::string& str) {
  int i = 0;
  int j = str.size()-1;

  while(i < j ) {
    if (str[i] != str[j]) {
      return false;
    }
    i++;
    j--;
  }
  return true;
}

void DecomposeRecurse(const string& input, int offset, vector<string>& currStrs, vector<vector<string>>& ans)
{
  if (offset == input.size()) {
    ans.emplace_back(currStrs);
    return;
  }

  for (int i = offset+1; i <= input.size(); i++) {
    std::string prefix = input.substr(offset, i-offset);
    if (IsPalindrome(prefix)) {
      currStrs.emplace_back(prefix);
      DecomposeRecurse(input, i, currStrs, ans);
      currStrs.pop_back();
    }
  }
}



vector<vector<string>> PalindromeDecompositions(const string& input) {
  vector<vector<string>> ans;
  vector<string> currStrs;

  DecomposeRecurse(input, 0, currStrs, ans);
  return ans;
}
bool Comp(vector<vector<string>> expected, vector<vector<string>> result) {
  std::sort(begin(expected), end(expected));
  std::sort(begin(result), end(result));
  return expected == result;
};

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"input"};
  return GenericTestMain(args, "enumerate_palindromic_decompositions.cc",
                         "enumerate_palindromic_decompositions.tsv",
                         &PalindromeDecompositions, &Comp, param_names);
}
