#include <list>
#include <iostream>
#include <vector>

class Number{
public:
    Number multiply(int val) {
        Number ans;
        auto iter = _nums.crbegin();
        int carry = 0;
        while(iter != _nums.crend()) {
            int curr = (*iter) * val;
            curr += carry;
            if (curr >= 10) {
                carry = curr / 10;
                curr %= 10; 
            } else {
                carry = 0;
            }
            ans.push_front(curr);
            ++iter;
        }
        if (carry != 0) {
            ans.push_front(carry);
        }

        return ans;
    }

    void push_front(int val) {
        _nums.push_front(val);
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
    std::list<int>::const_iterator getBegin() const {
        return _nums.cbegin();
    }

    std::list<int>::const_iterator getEnd() const {
        return _nums.cend();
    }
    std::list<int> _nums;
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

Number nthMultiple(long int n) {
    std::vector<Number> dp(1);
    int l2 = 0,l3 = 0,l5 = 0;
    //std::cout << "I am here" << std::endl;
    dp[0].push_front(1);
    
    for (int i = 1; i < n; i++) {
        Number twoMultiple = dp[l2].multiply(2);
        //std::cout << "Two multiple " << twoMultiple << std::endl;
        Number threeMultiple = dp[l3].multiply(3);
        //std::cout << "Three multiple " << threeMultiple << std::endl;
        Number fiveMultiple = dp[l5].multiply(5);
        //std::cout << "Five multiple " << fiveMultiple << std::endl;
        dp.push_back(std::min(std::min(twoMultiple, threeMultiple), fiveMultiple));
        //std::cout << "After comparison " << dp[i] << std::endl;
        if (dp[i] == twoMultiple) {
            l2++;
        }
        
        if (dp[i] == threeMultiple) {
            l3++;
        }
        
        if (dp[i] == fiveMultiple) {
            l5++;
        }            
    }
    
    return dp[n-1];
} 
// Driver code
int main(int argc, char* argv[])
{
    //std::cout << "Number of arguments " << argc;
    if (argc != 2) {
        std::cerr << "Invalid number of arguments " << std::endl;
        return -1;
    }
    std::cout << nthMultiple(strtol(argv[1], NULL, 10)) << std::endl;
 
    return 0;
}