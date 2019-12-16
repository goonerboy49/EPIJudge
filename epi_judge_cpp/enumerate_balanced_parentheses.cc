#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

void GenerateBalancedParenthesesInt(int numLeftNeeded, int numRightNeeded, const std::string& prefix, vector<string>& ans)
{
  if (!numRightNeeded) {
    ans.push_back(prefix);
    return;
  }

  if (numLeftNeeded > 0) {
    GenerateBalancedParenthesesInt(numLeftNeeded - 1, numRightNeeded, prefix + "(", ans);
  }

  if (numLeftNeeded < numRightNeeded) {
    GenerateBalancedParenthesesInt(numLeftNeeded, numRightNeeded - 1, prefix + ")", ans);
  }
}


vector<string> GenerateBalancedParentheses(int num_pairs) {
  vector<string> ans;
  GenerateBalancedParenthesesInt(num_pairs, num_pairs, "", ans);
  return ans;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num_pairs"};
  return GenericTestMain(args, "enumerate_balanced_parentheses.cc",
                         "enumerate_balanced_parentheses.tsv",
                         &GenerateBalancedParentheses,
                         &UnorderedComparator<vector<string>>, param_names);
}
