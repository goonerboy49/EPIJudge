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
  Number(int val) { _nums.push_back(char('0' + val)); }
  Number() {}
  ~Number() {
    _nums.clear();
    _nums.shrink_to_fit();
  }

  Number(const Number &o) : _nums(o._nums) {}
  Number(Number &&o) : _nums(std::move(o._nums)) {}
  Number &operator=(Number const &other) {
    _nums = other._nums;
    return *this;
  }

  Number &operator=(Number &&other) {
    _nums = std::move(other._nums);
    return *this;
  }

  /**
   * @brief Multiplication operation on the existing number.
   *
   * @param val
   */
  void multiply(int val) {
    auto iter = _nums.rbegin();
    int carry = 0;
    while (iter != _nums.rend()) {
      int curr = (*iter - '0') * val;
      curr += carry;
      *iter = curr % 10 + '0';
      carry = curr / 10;
      ++iter;
    }
    if (carry != 0) {
      _nums.insert(_nums.begin(), carry + '0');
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
    if (prime == 5 &&
        ((_nums.back() - '0') == 5 || (_nums.back() - '0') == 0)) {
      return true;
    } else if (prime == 3 && isMultipleOf3()) {
      return true;
    } else if (prime == 2 && (_nums.back() - '0') % 2 == 0) {
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
    auto sumFn = [](long int sum, char c) { return sum + (c - '0'); };
    long int sum = std::accumulate(_nums.begin(), _nums.end(), 0, sumFn);
    return sum % 3 == 0;
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

  // Friend function to output the number
  friend std::ostream &operator<<(std::ostream &out, const Number &number);

private:
  std::string::const_iterator getBegin() const { return _nums.cbegin(); }

  std::string::const_iterator getEnd() const { return _nums.cend(); }

  /**
   * Digits that represent a number, most significant bit is present at the
   * front. Storing individual digits as a node in the vector helps in
   * representing a very large number.
   *
   * @note Storage for a result of multiplication for a number
   * with a high number of digits might seem to naturally fit the usecase for an
   * std::list. However a list node in addition to the data also includes 2
   * pointers to the next and previous nodes which causes memory in efficiency.
   * More importantly for the problem at hand allocaiton of new nodes in the
   * list causes Random memory causes causing a significant degradation in
   * performance when compared to using a vector for the same number of digits.
   */
  std::string _nums;
};

std::ostream &operator<<(std::ostream &out, const Number &number) {
  auto iter = number.getBegin();
  while (iter != number.getEnd()) {
    out << *iter;
    ++iter;
  }
  return out;
}

/**
 * @brief Shrink the size of the vector if the size is > MAX_SIZE
 *
 * @param multiples Vector of multiples of a prime number.
 * @param front Index of first unprocessed number in @c multiples. This value is
 * updated after the pruning operation.
 * @param maxSize Vector size at which point pruning will be applied.
 */
void pruneUsed(std::vector<Number> &multiples, long int &front,
               long int maxSize) {
  if (multiples.size() >= maxSize) {
    multiples.erase(multiples.begin(), multiples.begin() + front);
    multiples.shrink_to_fit();
    front = 0;
  }
}

/**
 * @brief Generate the nth number that can be factored with given list
 * of prime numbers.
 *
 * @param n Nth number.
 * @return Nth Number.
 */
Number nthMultiple(long int n) {
  long int const MAX_SIZE = 1000000;
  long int l2 = 0, l3 = 0, l5 = 0;
  std::vector<Number> twoMultiples;
  twoMultiples.emplace_back(std::move(Number(2)));

  std::vector<Number> threeMultiples(1);
  threeMultiples[0] = std::move(Number(3));

  std::vector<Number> fiveMultiples(1);
  fiveMultiples[0] = std::move(Number(5));

  Number ans;
  Number maxSoFar(1);
  for (long int i = 1;; i++) {
    //std::cout << "2 " << twoMultiples[l2] << " 3 " << threeMultiples[l3] << " 5 " << fiveMultiples[l5] << "\n";
    Number nextNumber = std::min(std::min(l2 < twoMultiples.size() ? twoMultiples[l2] : Number(), l3 < threeMultiples.size() ? threeMultiples[l3] : Number()),
                                 l5 < fiveMultiples.size() ? fiveMultiples[l5]: Number());
    //std::cout << "Next num " << nextNumber << "\n";                          
    if (i == n - 1) {
      ////std::cout << "Found ans " << nextNumber;
      ans = nextNumber;
      break;
    }

    // One candidate is processed in this iteration
    long int unprocessed = (twoMultiples.size() - l2) +
                           (threeMultiples.size() - l3) +
                           (fiveMultiples.size() - l5) - 1;
    long int const remaining = n - i;

    // Save the multiple of a prime in it's respective vector of candidates.
    // Hence duplicate candidates are not generated in each of the vectors.
    // For instance if the next candidate is 2, save 4 in twoMultiples, save 6
    // in threeMultiples and save 10 in fiveMultiples. Note when 5 is the next
    // chosen element, candidate 10 (5*2) is not added to the list of candidates
    // as it was previously added to fiveMultiples when 2 was chosen in a
    // previous iteration. Hence check if the number is a multiple starting from
    // the highest prime number.
    if (nextNumber.isMultiple(5)) {
      ++l5;
      Number fiveMultiple = nextNumber;
      fiveMultiple.multiply(5);

      // Add to the vector of candidates only if the number is lesser than the
      // max number seen so far if the number of unprocessed candidates is
      // lesser than the remaining numbers to the target.
      // This optimization prevents increasing the size of vectors beyond the
      // necessary candidates to reach the target n.
      if (unprocessed < remaining || fiveMultiple < maxSoFar) {
        maxSoFar = std::max(maxSoFar, fiveMultiple);
        fiveMultiples.emplace_back(fiveMultiple);
      }
    } else if (nextNumber.isMultiple(3)) {
      ++l3;
      Number threeMultiple = nextNumber;
      threeMultiple.multiply(3);
      if (unprocessed < remaining || threeMultiple < maxSoFar) {
        maxSoFar = std::max(threeMultiple, maxSoFar);
        threeMultiples.emplace_back(threeMultiple);
        unprocessed++;

        Number fiveMultiple = nextNumber;
        fiveMultiple.multiply(5);
        if (unprocessed < remaining || fiveMultiple < maxSoFar) {
          maxSoFar = std::max(fiveMultiple, maxSoFar);
          fiveMultiples.emplace_back(fiveMultiple);
        }
      }
    } else {
      ++l2;
      Number twoMultiple = nextNumber;
      twoMultiple.multiply(2);

      if (unprocessed < remaining || twoMultiple < maxSoFar) {
        maxSoFar = std::max(twoMultiple, maxSoFar);
        twoMultiples.emplace_back(twoMultiple);
        unprocessed++;

        Number threeMultiple = nextNumber;
        threeMultiple.multiply(3);
        if (unprocessed < remaining || threeMultiple < maxSoFar) {
          maxSoFar = std::max(threeMultiple, maxSoFar);
          threeMultiples.emplace_back(threeMultiple);
          unprocessed++;

          Number fiveMultiple = nextNumber;
          fiveMultiple.multiply(5);
          if (unprocessed < remaining || fiveMultiple < maxSoFar) {
            maxSoFar = std::max(fiveMultiple, maxSoFar);
            fiveMultiples.emplace_back(fiveMultiple);
          }
        }
      }
    }

    pruneUsed(twoMultiples, l2, MAX_SIZE);
    pruneUsed(threeMultiples, l3, MAX_SIZE);
    pruneUsed(fiveMultiples, l5, MAX_SIZE);
  }

  return ans;
}

// Driver code
int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Invalid number of arguments " << std::endl;
    return -1;
  }
  std::cout << nthMultiple(strtol(argv[1], NULL, 10)) << std::endl;

  return 0;
}