#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

double FindSalaryCap(int target_payroll, vector<int> current_salaries) {
  double sum_so_far = 0;
  std::sort(current_salaries.begin(), current_salaries.end());

  double salaryCap = -1;
  for (int i = 0; i < current_salaries.size(); i++) {
    double total_payroll = sum_so_far + (current_salaries.size() - i) * current_salaries[i];
    if (total_payroll >= target_payroll) {
      salaryCap = (target_payroll - sum_so_far) / (current_salaries.size() - i);
      break;
    }
    sum_so_far += current_salaries[i];
  }


  return salaryCap;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"target_payroll", "current_salaries"};
  return GenericTestMain(args, "find_salary_threshold.cc",
                         "find_salary_threshold.tsv", &FindSalaryCap,
                         DefaultComparator{}, param_names);
}
