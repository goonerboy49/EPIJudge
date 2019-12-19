#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;


int LevenshteinDistance(const string& A, const string& B) {
  int m = A.length();
  int n = B.length();
  vector<vector<int>> dp(m+1, vector<int>(n+1));

  for(int i = 0; i <=m; i++) {
    dp[i][0] = i;
  }

  for(int i = 0; i <=n; i++) {
    dp[0][i] = i;
  }

  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      int removeWord1 = dp[i-1][j] + 1;
      int removeWord2 = dp[i][j-1] + 1;
      int removeBoth = dp[i-1][j-1];
      if (A[i-1] != B[j-1]) {
        removeBoth += 1;
      }

      dp[i][j] = std::min(removeWord1, std::min(removeWord2, removeBoth));
    }
  }
  return dp[m][n];
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "B"};
  return GenericTestMain(args, "levenshtein_distance.cc",
                         "levenshtein_distance.tsv", &LevenshteinDistance,
                         DefaultComparator{}, param_names);
}
