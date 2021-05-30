#include <list>
#include <iostream>
#include <numeric>
#include <queue>
#include <vector>

/**
 * Represents a number.
 */
class Number {
public:
    // Initialize the number with a digit.
    /**
     * @brief Construct a new Number object
     * 
     * @param val Intialize with a digit
     * 
     * @note val < 9
     */
    Number(int val) {_nums.push_front(val);}

    /**
     * @brief Multiplication operation on the existing number.
     * 
     * @param val 
     */
    void multiply(int val) {
        auto iter = _nums.rbegin();
        int carry = 0;
        while(iter != _nums.rend()) {
            int curr = (*iter) * val;
            curr += carry;
            if (curr >= 10) {
                carry = curr / 10;
                curr %= 10; 
            } else {
                carry = 0;
            }
            *iter = curr;
            ++iter;
        }
        if (carry != 0) {
            _nums.push_front(carry);
        }
    }

    /**
     * @brief Return the number of digits in this number.
     * 
     * @return int 
     */
    int size() const {
        return _nums.size();
    }

    /**
     * @brief Is number a multiple of 3.
     * 
     * @return true if number is a multiple of 3.
     * @return false otherwise.
     */
    bool isMultipleOf3() const {
        int sum = std::accumulate(_nums.begin(), _nums.end(), 0);
        return sum % 3 == 0;
    }

    /**
     * @brief Is number a multiple of 5.
     * 
     * @return true if number is a multiple of 5.
     * @return false otherwise.
     */
    bool isMultipleOf5() const {
        return _nums.back() == 5 || _nums.back() == 0;
    }

    /**
     * @brief > Operator to sort the minHeap in ascending order.
     * 
     * @param b Other element in the minHeap.
     * @return true 
     * @return false 
     */
    bool operator>(Number const& b) const {
        if (_nums.size() < b.size()) {
            return false;
        } else if (_nums.size() > b.size()) {
            return true;
        } else {
            auto iter1 = _nums.begin();
            auto iter2 = b.getBegin();

            while(iter1 != _nums.end() && iter2 != b.getEnd() && *iter1 == *iter2) {
                ++iter1;
                ++iter2;
            }

            return *iter1 > *iter2;
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
    std::list<int>::const_iterator getBegin() const {
        return _nums.cbegin();
    }

    std::list<int>::const_iterator getEnd() const {
        return _nums.cend();
    }

    // Digits that represent a number, most significant bit is present at the front.
    // Storing individual digits as a node in the list helps in representing a very large number.
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
    std::priority_queue<Number, std::vector<Number>, std::greater<Number> > minHeap;
    Number twoMultiple(2);
    Number threeMultiple(3);
    Number fiveMultiple(5);
    minHeap.push(twoMultiple);
    minHeap.push(threeMultiple);
    minHeap.push(fiveMultiple);
    Number maxSoFar = fiveMultiple;
    
    for (int i = 1; i < n-1; i++) {
        Number top = minHeap.top();
        minHeap.pop();

        if (top.isMultipleOf5()) {
            Number fiveMultiple = top;
            fiveMultiple.multiply(5);
            
            if (minHeap.size() < n-i-1 || maxSoFar > fiveMultiple) {
                minHeap.push(fiveMultiple);
                maxSoFar = fiveMultiple > maxSoFar ? fiveMultiple : maxSoFar;
            }
        } else if (top.isMultipleOf3()) {
            Number fiveMultiple = top;
            fiveMultiple.multiply(5);
            if (minHeap.size() < n-i-1 || maxSoFar > fiveMultiple) {
                minHeap.push(fiveMultiple);
                maxSoFar = fiveMultiple > maxSoFar ? fiveMultiple : maxSoFar;
            }

            Number threeMultiple = top;
            threeMultiple.multiply(3);
            if (minHeap.size() < n-i-1 || maxSoFar > threeMultiple) {
                minHeap.push(threeMultiple);
                maxSoFar = threeMultiple > maxSoFar ? threeMultiple : maxSoFar;
            }
        } else {
            Number fiveMultiple = top;
            fiveMultiple.multiply(5);
            if (minHeap.size() < n-i-1 || maxSoFar > fiveMultiple) {
                minHeap.push(fiveMultiple);
                maxSoFar = fiveMultiple > maxSoFar ? fiveMultiple : maxSoFar;
            }

            Number threeMultiple = top;
            threeMultiple.multiply(3);
            if (minHeap.size() < n-i-1 || maxSoFar > threeMultiple) {
                minHeap.push(threeMultiple);
                maxSoFar = threeMultiple > maxSoFar ? threeMultiple : maxSoFar;
            }

            Number twoMultiple = top;
            twoMultiple.multiply(2);
            if (minHeap.size() < n-i-1 || maxSoFar > twoMultiple) {
                minHeap.push(twoMultiple);
                maxSoFar = twoMultiple > maxSoFar ? twoMultiple : maxSoFar;
            }
        }
    }
    return minHeap.top();
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