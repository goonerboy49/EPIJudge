#include <string>
#include <unordered_set>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::unordered_set;
using std::vector;

vector<string> DecomposeIntoDictionaryWords(
    const string& domain, const unordered_set<string>& dictionary) {
  vector<int> lastIdx(domain.size(), -1);
  for (int end = 0; end < domain.size(); end++) {
    if (dictionary.count(domain.substr(0, end + 1))) {
      //std::cout << "Last idx of end " << end << " set to 0" << std::endl;
      lastIdx[end] = 0;
      continue;
    }
    for (int start = 0; start <= end; start++) {
      //std::cout << "Checking substring " << domain.substr(start, end-start+1) << std::endl;
      if (start > 0 && lastIdx[start-1] != -1 && dictionary.find(domain.substr(start, end-start+1)) != dictionary.end()) {
        //std::cout << "last idx of end " << end << " set to " << start << std::endl;
        lastIdx[end] = start;
        break; 
      }
    }
  }
  vector<string> ans;
  int endIdx = domain.size()-1;
  int startIdx = lastIdx[endIdx];
  while(startIdx != -1) {
    //std::cout << "Adding " << domain.substr(startIdx, endIdx-startIdx+1) << " to result" << std::endl;
    ans.emplace_back(domain.substr(startIdx, endIdx-startIdx+1));
    if (startIdx == 0) {
      break;
    }
    endIdx = startIdx-1;
    startIdx = lastIdx[endIdx];
  }

  std::reverse(ans.begin(), ans.end());

  return ans;
}
void DecomposeIntoDictionaryWordsWrapper(
    TimedExecutor& executor, const string& domain,
    const unordered_set<string>& dictionary, bool decomposable) {
  vector<string> result = executor.Run(
      [&] { return DecomposeIntoDictionaryWords(domain, dictionary); });
  if (!decomposable) {
    if (!result.empty()) {
      throw TestFailure("domain is not decomposable");
    }
    return;
  }

  if (std::any_of(std::begin(result), std::end(result),
                  [&](const std::string& s) { return !dictionary.count(s); })) {
    throw TestFailure("Result uses words not in dictionary");
  }

  if (std::accumulate(std::begin(result), std::end(result), string()) !=
      domain) {
    throw TestFailure("Result is not composed into domain");
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "domain", "dictionary",
                                       "decomposable"};
  return GenericTestMain(args, "is_string_decomposable_into_words.cc",
                         "is_string_decomposable_into_words.tsv",
                         &DecomposeIntoDictionaryWordsWrapper,
                         DefaultComparator{}, param_names);
}
