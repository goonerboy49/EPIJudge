#include <string>
#include <queue>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

class Solution {
public:
    /**
     * @param words: a list of words
     * @return: a string which is correct order
     */
    string alienOrder(vector<string> &words) {
        int curr = 0;

        std::unordered_map<char, vector<char> > graph;
        std::unordered_map<char, int> indegrees;
        while(curr+1 < words.size()) {
            string& first = words[curr];
            string& second = words[curr+1];
            int i = 0;
            while (i < first.size() && i < second.size() && first[i] == second[i]) {
                i++;
            }
            
            if (i < first.size() && i < second.size()) {
                char from = first[i];
                char to = second[i];

                graph[from].push_back(to);
                if (indegrees.find(from) == indegrees.end()) {
                    indegrees[from] = 0;
                }
                indegrees[to]++;
            } 
            ++curr;
        }

        std:queue<char> bfs;
        for (auto iter : indegrees) {
            if (iter.second == 0) {
                bfs.push(iter.first);
            }
        }

        string ans;
        while(!bfs.empty()) {
            char front = bfs.front();
            bfs.pop();
            ans.push_back(front);
            vector<char> nextChars = graph[front];
            for (char c : nextChars) {
                indegrees[c]--;
                if (indegrees[c] == 0) {
                    bfs.push(c);
                }
            }
        }

        return ans;
    }
};

int main(int argc, char* argv[]) {
  std::vector<std::string> words = {"wrt","wrf","er","ett","rftt"};
  Solution sol;
  std::cout << sol.alienOrder(words);
  return 0;
}