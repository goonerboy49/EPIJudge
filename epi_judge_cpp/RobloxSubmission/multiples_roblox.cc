#include <iostream>
#include <list>
#include <numeric>
#include <vector>

/**
 * Represents a number.
 */
class Number {
public:
  /**
   * @brief Construct a new Number object
   *
   * @param val Intialize with a digit
   *
   * @note val <= 9
   */
  Number(int val) { _nums.push_back(val); }
  Number() {}

//   Number &operator=(Number &&other) {
//     if (this != &other) {
//       _nums = other._nums;
//     }

//     return *this;
//   }

//   Number(Number &&other) { _nums = other._nums; }

//   Number(const Number &other) {
//     std::copy(other._nums.begin(), other._nums.end(),
//               std::back_inserter(_nums));
//   }

//   Number &operator=(const Number &other) {
//     if (this != &other) {
//       std::vector<int>().swap(_nums);
//       std::copy(other._nums.begin(), other._nums.end(),
//                 std::back_inserter(_nums));
//     }
//     return *this;
//   }

  /**
   * @brief Multiplication operation on the existing number.
   *
   * @param val
   */
  void multiply(int val) {
    auto iter = _nums.rbegin();
    int carry = 0;
    while (iter != _nums.rend()) {
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
      _nums.insert(_nums.begin(), carry);
    }
  }

  /**
   * @brief Return the number of digits in this number.
   *
   * @return int
   */
  int size() const { return _nums.size(); }

  /**
   * @brief Returns true if the Number is a multiple of given prime.
   *
   * @param prime
   * @return true
   * @return false
   *
   * @note Currently supports finding multiples of prime numbers {2,3,5}
   */
  bool isMultiple(int prime) const {
    if (prime == 5 && (_nums.back() == 5 || _nums.back() == 0)) {
      return true;
    } else if (prime == 3 && isMultipleOf3()) {
      return true;
    } else if (prime == 2 && _nums.back() % 2 == 0) {
      return true;
    }

    return false;
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
   * @brief > Operator to sort the minHeap in ascending order.
   *
   * @param b Other element in the minHeap.
   * @return true
   * @return false
   */
  bool operator==(Number const &b) const {
    if (_nums.size() != b.size()) {
      return false;
    }

    auto iter1 = _nums.begin();
    auto iter2 = b.getBegin();

    while (iter1 != _nums.end() && iter2 != b.getEnd() && *iter1 == *iter2) {
      ++iter1;
      ++iter2;
    }

    return iter1 == _nums.end() && iter2 == b.getEnd() ? true : false;
  }

  bool operator<(Number const &b) const {
    if (_nums.size() == 0 || b.size() == 0) {
      return _nums.size() == 0 ? false : true;
    } else if (_nums.size() < b.size()) {
      return true;
    } else if (_nums.size() > b.size()) {
      return false;
    } else {
      auto iter1 = _nums.begin();
      auto iter2 = b.getBegin();

      while (iter1 != _nums.end() && iter2 != b.getEnd() && *iter1 == *iter2) {
        ++iter1;
        ++iter2;
      }

      return *iter1 < *iter2;
    }
  }

  friend std::ostream &operator<<(std::ostream &out, const Number &number);

private:
  std::vector<int>::const_iterator getBegin() const { return _nums.cbegin(); }

  std::vector<int>::const_iterator getEnd() const { return _nums.cend(); }

  // Digits that represent a number, most significant bit is present at the
  // front. Storing individual digits as a node in the vector helps in
  // representing a very large number.
  std::vector<int> _nums;
};

std::ostream &operator<<(std::ostream &out, const Number &number) {
  auto iter = number.getBegin();
  while (iter != number.getEnd()) {
    out << *iter;
    ++iter;
  }

  return out;
}

int pruneUsed(std::vector<Number> &multiples, int currIdx, int remaining, int num) {
  if (multiples.size() > 1000000) {
    multiples.erase(multiples.begin(), multiples.begin() + currIdx);
    multiples.shrink_to_fit();
    currIdx = 0;
  }

  return currIdx;
}

/**
 * @brief Generate the nth number that can be factored with given list
 * of prime numbers.
 *
 * @param n Nth number.
 * @return Nth Number.
 */
Number nthMultiple(long int n) {
  int l2 = 0, l3 = 0, l5 = 0;
  std::vector<Number> twoMultiples;
  twoMultiples.emplace_back(std::move(Number(2)));

  std::vector<Number> threeMultiples;
  threeMultiples.emplace_back(std::move(Number(3)));

  std::vector<Number> fiveMultiples;
  fiveMultiples.emplace_back(std::move(Number(5)));

  Number ans(0);
  int count=0;
  Number maxSoFar(1);
  for (int i = 1;; i++) {
    Number nextNumber = std::min(std::min(l2 < twoMultiples.size() ? twoMultiples[l2] : Number(), l3 < threeMultiples.size() ? threeMultiples[l3]: Number()),
                                 l5 < fiveMultiples.size() ? fiveMultiples[l5]: Number());
    if (i == n - 1) {
      ans = nextNumber;
      return nextNumber;
    }

    int unprocessed = twoMultiples.size() - l2 + threeMultiples.size() - l3 + fiveMultiples.size() - l5 - 1;
    int remaining = n-i;

    if (nextNumber.isMultiple(5)) {
      l5++;
      Number fiveMultiple = nextNumber;
      fiveMultiple.multiply(5);
      if (unprocessed < remaining || fiveMultiple < maxSoFar) {
        maxSoFar = std::max(maxSoFar, fiveMultiple);
        fiveMultiples.emplace_back(std::move(fiveMultiple));
      }
    } else if (nextNumber.isMultiple(3)) {
      ++l3;
      Number threeMultiple = nextNumber;
      Number fiveMultiple = nextNumber;
      threeMultiple.multiply(3);
      fiveMultiple.multiply(5);
      if (unprocessed < remaining || threeMultiple < maxSoFar) {
        maxSoFar = std::max(threeMultiple, maxSoFar);
        threeMultiples.emplace_back(std::move(threeMultiple));
        unprocessed++;
        if (unprocessed < remaining || fiveMultiple < maxSoFar) {
          maxSoFar = std::max(fiveMultiple, maxSoFar);
          fiveMultiples.emplace_back(std::move(fiveMultiple));
        }
      }      
    } else {
      ++l2;
      Number twoMultiple = nextNumber;
      Number threeMultiple = nextNumber;
      Number fiveMultiple = nextNumber;
      twoMultiple.multiply(2);
      threeMultiple.multiply(3);
      fiveMultiple.multiply(5);

      if (unprocessed < remaining || twoMultiple < maxSoFar) {
        maxSoFar = std::max(twoMultiple, maxSoFar);
        twoMultiples.emplace_back(std::move(twoMultiple));
        unprocessed++;
        if (unprocessed < remaining || threeMultiple < maxSoFar) {
          maxSoFar = std::max(threeMultiple, maxSoFar);
          threeMultiples.emplace_back(std::move(threeMultiple));
          unprocessed++;
          if (unprocessed < remaining || fiveMultiple < maxSoFar) {
            maxSoFar = std::max(fiveMultiple, maxSoFar);
            fiveMultiples.emplace_back(std::move(fiveMultiple));
          }
        }
      }
    }

    l2 = pruneUsed(twoMultiples, l2, n-i, 2);
    l3 = pruneUsed(threeMultiples, l3, n-i, 3);
    l5 = pruneUsed(fiveMultiples, l5, n-i, 5);
  }

  return ans;
}

// Driver code
int main(int argc, char *argv[]) {
  ////std::cout << "Number of arguments " << argc;
  if (argc != 2) {
    std::cerr << "Invalid number of arguments " << std::endl;
    return -1;
  }
 std::cout << nthMultiple(strtol(argv[1], NULL, 10)) << std::endl;

  return 0;
}