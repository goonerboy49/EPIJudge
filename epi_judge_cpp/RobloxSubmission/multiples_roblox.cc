#include <iostream>
#include <vector>

class Number{
public:
    Number(int val) { _nums.push_back(val);}
    Number() {}
    ~Number() { 
      _nums.clear();
      _nums.shrink_to_fit();
    }

    Number(const Number& o) : _nums(o._nums) {}
    Number(Number&& o) : _nums(std::move(o._nums)) {}
    Number& operator=(Number const& other)
    {
         _nums = other._nums;
         return *this;
    }

    Number& operator=(Number&& other)
    {
      _nums = std::move(other._nums);
      return *this;
    }

    void multiply(int val) {
      auto iter = _nums.rbegin();
      int carry = 0;
      while(iter != _nums.rend()) {
          int curr = (*iter) * val;
          curr += carry;
          *iter = curr % 10;
          carry = curr/10;
          ++iter;
      }
      if (carry != 0) {
          _nums.insert(_nums.begin(), carry);
      }
    }

    int size() const {
        return _nums.size();
    }

    bool operator<(Number const& b) const {
        if (_nums.size() < b.size()) {
            return true;
        } else if (_nums.size() > b.size()) {
            return false;
        } else {
            auto iter1 = _nums.begin();
            auto iter2 = b.getBegin();

            while(iter1 != _nums.end() && iter2 != b.getEnd() && *iter1 == *iter2) {
                ++iter1;
                ++iter2;
            }

            return *iter1 < *iter2;
        }
    }

    bool operator==(Number const& b) const {
        if (_nums.size() != b.size()) {
          return false;
        }
        auto iter1 = _nums.begin();
        auto iter2 = b.getBegin();

        while(iter1 != _nums.end() && iter2 != b.getEnd() && *iter1 == *iter2) {
            ++iter1;
            ++iter2;
        }

        return iter1 == _nums.end() && iter2 == b.getEnd() ? true : false;
    }

    friend std::ostream& operator<< (std::ostream &out, const Number& number);
private:
    std::vector<int>::const_iterator getBegin() const {
        return _nums.cbegin();
    }

    std::vector<int>::const_iterator getEnd() const {
        return _nums.cend();
    }
    std::vector<int> _nums;
};

std::ostream& operator<< (std::ostream &out, const Number& number) {
    auto iter = number.getBegin();
    while (iter != number.getEnd())
    {
        out << *iter;
        ++iter;
    }

    return out;
}

void pruneUsed(std::vector<Number>& dp, long int& l2, long int& l3, long int& l5, long int& last) {
  long int minIdx = std::min(std::min(l2, l3), l5);

  if (last == 999999) {
    dp.erase(dp.begin(), dp.begin() + minIdx);
    dp.shrink_to_fit();
    dp.resize(1000000);
    l2 -= minIdx;
    l3 -= minIdx;
    l5 -= minIdx;
    last -= minIdx;
  }
}

Number nthMultiple(long int n) {
    std::vector<Number> dp(1000000);
    long int l2 = 0,l3 = 0,l5 = 0;
    dp[0] = Number(1);
    
    long int last = 1;
    for (long int i = 1; i < n; i++) {
        //std::cout << "Before 2 " << std::endl;
        Number twoMultiple = dp[l2];
        twoMultiple.multiply(2);
        //std::cout << "Before 3 " << std::endl;
        Number threeMultiple = dp[l3];
        threeMultiple.multiply(3);
        //std::cout << "Before 5 " << std::endl;
        Number fiveMultiple = dp[l5];
        fiveMultiple.multiply(5);
        //std::cout << "Before min assignment " << std::endl;
        dp[last] = std::move(std::min(std::min(twoMultiple, threeMultiple), fiveMultiple));
        ////std::cout << "next number " << dp[last] << std::endl;
        if (dp[last] == twoMultiple) {
            l2++;
        }
        
        if (dp[last] == threeMultiple) {
            l3++;
        }
        
        if (dp[last] == fiveMultiple) {
            l5++;
        }
        last++;

        pruneUsed(dp, l2, l3, l5, last);            
    }
    
    return std::move(dp[last-1]);
} 
// Driver code
int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Invalid number of arguments " << std::endl;
        return -1;
    }
    std::cout << nthMultiple(strtol(argv[1], NULL, 10)) << std::endl;
 
    return 0;
}