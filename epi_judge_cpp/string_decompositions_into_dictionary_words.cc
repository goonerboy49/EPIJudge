#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

using WordFreqMap = std::unordered_map<string, int>;

bool IsMatch(const string& s, int startIdx, int unitSize, int numWords, WordFreqMap& freqMap) {

  for(int i = 0; i < numWords; i++) {
    auto currWord = s.substr(startIdx + i * unitSize, unitSize);
    auto iter = freqMap.find(currWord);
  }

}


vector<int> FindAllSubstrings(const string& s, const vector<string>& words) {
  
  return {};
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s", "words"};
  return GenericTestMain(args, "string_decompositions_into_dictionary_words.cc",
                         "string_decompositions_into_dictionary_words.tsv",
                         &FindAllSubstrings, DefaultComparator{}, param_names);
}
