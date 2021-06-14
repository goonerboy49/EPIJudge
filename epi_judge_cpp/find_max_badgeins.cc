#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>

using namespace std;

std::vector<std::pair<std::string, std::vector<string>>> 
FindTimesOver(int n, vector<pair<string, string>> const& badge_times) 
{
  std::unordered_map<string, std::vector<int>> buckets;
  
  for (auto badge_time : badge_times) {
    string person = badge_time.first;
    int time = stoi(badge_time.second);
    
    buckets[person].emplace_back(time);
  }
  
  std::vector<std::pair<std::string, std::vector<string>>> ans;
  
  for (auto bucket : buckets) {
    std::vector<int> times = bucket.second;
    std::sort(times.begin(), times.end());
    string person = bucket.first;
    cout << "For person " << person << endl;
    
    int first = 0;
    while(first <= times.size() - n) {
      int last = first+1;
      int startTime = times[first];
      std::vector<string> inHourTimes;
      int diffInCurrentHour = 60 - startTime % 100;
      int currentHour = startTime / 100;
      inHourTimes.emplace_back(to_string(startTime));
      while(last < times.size()) {
        int lastTime = times[last];
        int diff = (lastTime/100 == currentHour) ? (lastTime - startTime) : (diffInCurrentHour + lastTime % 100);
        if (diff <= 60) {
          inHourTimes.emplace_back(to_string(lastTime));  
          ++last;
        } else {
          break;
        }
      }
      
      if (last - first >= n) {
        ans.emplace_back(std::make_pair(person, inHourTimes));
        break;
      }
      ++first;
    }
  }
  
  return ans;
}


int main() {
  cout << "Hello, world!" << endl;
  cout << "This is a fully functioning C++ environment." << endl;
  
  vector<pair<string, string>> badge_times = {
    {"Paul",     "1355"},
    {"Jennifer", "1910"},
    {"John",      "835"},
    {"John",      "830"},
    {"Paul",     "1315"},
    {"John",     "1615"},
    {"John",     "1640"},
    {"Paul",     "1405"},
    {"John",      "855"},
    {"John",      "930"},
    {"John",      "915"},
    {"John",      "730"},
    {"John",      "940"},
    {"Jennifer", "1335"},
    {"Jennifer",  "730"},
    {"John",     "1630"},
    {"Jennifer",    "5"}
  };

  auto result = FindTimesOver(3, badge_times);
  
  for (auto res: result) {
    std::cout << res.first << ": ";
    for (string str : res.second) {
      cout << str << " ";
    }
    cout << "\n";
  }
  
  return 0;
}