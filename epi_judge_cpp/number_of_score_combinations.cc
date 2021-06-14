#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

/**
 * Notes - A 1D matrix of size final_score + 1 to memoize the
 * results so far upto scores[i]
 * 
 * In the for loop
 * dp[j] is the number of combinations using the first i scores
 * to reach a final score of j. The recursive formula
 * withoutThisPlay = dp[j] from the previous row
 * withThisPlay = dp[j - score[i]] if score[i] <= j
 * dp[j]= withThisPlay + withoutThisPlay
 * 
 * Final number of combinations = dp[finalScore]
 */ 
int NumCombinationsForFinalScore(int final_score,
                                 const vector<int>& individual_play_scores) {
    std::vector<int> dp(final_score + 1, 0);
  
    dp[0] = 1; // There is only one way to reach final score 0
    for (int i = 0; i < individual_play_scores.size(); i++) {
      for (int j = 1; j <= final_score; j++) {
        int withoutThisPlay = dp[j];
        int withThisPlay = j >= individual_play_scores[i] ? dp[j-individual_play_scores[i]] : 0;
        dp[j] = withoutThisPlay + withThisPlay;
      }
    }

  return dp[final_score];
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"final_score", "individual_play_scores"};
  return GenericTestMain(args, "number_of_score_combinations.cc",
                         "number_of_score_combinations.tsv",
                         &NumCombinationsForFinalScore, DefaultComparator{},
                         param_names);
}
