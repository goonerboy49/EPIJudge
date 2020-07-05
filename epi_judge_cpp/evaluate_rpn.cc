#include <functional>
#include <map>
#include <stack>
#include <string>
#include <unordered_map>

#include "test_framework/generic_test.h"
using std::string;
int Evaluate(const string& expression) {
  std::stack<int> operands;
  const std::unordered_map<std::string, std::function<int(int, int)>> kOperations = {
    {"+", [](int x, int y) {return x+y;}},
    {"-", [](int x, int y) {return x-y;}},
    {"*", [](int x, int y) {return x*y;}},
    {"/", [](int x, int y) {return x/y;}}
  };

  std::stringstream ss(expression);
  std::string nextStr;

  while(getline(ss, nextStr, ',')) {
    if (kOperations.count(nextStr)) {
      int y = operands.top();
      operands.pop();
      int x = operands.top();
      operands.pop();
      operands.emplace(kOperations.at(nextStr)(x,y));
    } else {
      operands.emplace(stoi(nextStr));
    }
  }
  
  return operands.top();
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"expression"};
  return GenericTestMain(args, "evaluate_rpn.cc", "evaluate_rpn.tsv", &Evaluate,
                         DefaultComparator{}, param_names);
}
