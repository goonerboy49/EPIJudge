#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

vector<string> JustifyText(const vector<string>& words, int L) {
  vector<string> ans;
  vector<string> currLine;
  int currLineSize = 0;
  for(auto& word : words) {
    if (currLine.size() + word.size() + currLineSize > L) {
      for (int i = 0; i < (L - currLineSize); i++) {
        currLine[i % std::max(int(currLine.size()-1), 1)] += ' ';
      }

      std::cout << "Adding a line " << std::endl;
      for (auto& currWord : currLine) {
        std::cout << currWord << " " << currWord.size() << std::endl;
        ans.emplace_back(currWord);
      }
      std::cout << "\n" << std::endl;

      currLine.clear();
      currLineSize = 0;
    }
    currLineSize += word.size();
    currLine.emplace_back(word);
  }

  std::cout << "Adding last line " << std::endl;
  string lastWord;

  for (auto& currWord : currLine) {
    lastWord = lastWord + ' ' + currWord;
  }
  //std::cout << "Last word " << lastWord << currLineSize << " " << currLine.size() << std::endl;
  for (int i = 0; i < (L-currLineSize-(currLine.size()-1)); i++) {
    lastWord += ' ';
  }

  std::cout << "lastword is " << lastWord << std::endl;
  ans.emplace_back(lastWord);

  return ans;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"words", "L"};
  return GenericTestMain(args, "left_right_justify_text.cc",
                         "left_right_justify_text.tsv", &JustifyText,
                         DefaultComparator{}, param_names);
}
