#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

/**
 * Notes - A 2D matrix to memoize the results so far
 * dp[i][j] is the number of combinations using the first 1 scores
 * to reach a final score of j. The recursive formula
 * withThisPlay = dp[i][j-score[i]] if j >= score[i]
 * withoutThisPlay = dp[i - 1][j] if i > 0
 * dp[i][j]= withThisPlay + withoutThisPlay
 * 
 * Final number of combinations = dp[numScores-1][finalScore]
 */ 
int NumCombinationsForFinalScore(int final_score,
                                 const vector<int>& individual_play_scores) {
    std::vector<std::vector<int>> dp(individual_play_scores.size(), vector<int>(final_score + 1, 0));

    for (int i = 0; i < individual_play_scores.size(); i++) {
      for (int j = 0; j <= final_score; j++) {
        if (j == 0) {
          dp [i][j] = 1; // There is only one way to reach final score 0
          continue;
        }
        int withoutThisPlay = i > 0 ? dp[i-1][j] : 0;
        int withThisPlay = j >= individual_play_scores[i] ? dp[i-1][j-individual_play_scores[i]] : 0;
        dp[i][j] = withoutThisPlay + withThisPlay;
      }
    }

  return dp[individual_play_scores.size() - 1][final_score];
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"final_score", "individual_play_scores"};
  return GenericTestMain(args, "number_of_score_combinations.cc",
                         "number_of_score_combinations.tsv",
                         &NumCombinationsForFinalScore, DefaultComparator{},
                         param_names);
}
