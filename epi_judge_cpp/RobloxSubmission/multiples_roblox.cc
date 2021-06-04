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
  Number(int val) { _numStr.push_back(char('0' + val)); }
  Number() {}
  ~Number() {
    _numStr.clear();
    _numStr.shrink_to_fit();
  }

  Number(const Number &o) : _numStr(o._numStr) {}
  Number(Number &&o) : _numStr(std::move(o._numStr)) {}
  Number &operator=(Number const &other) {
    _numStr = other._numStr;
    return *this;
  }

  Number &operator=(Number &&other) {
    _numStr = std::move(other._numStr);
    return *this;
  }

  /**
   * @brief Multiplication operation on the existing number.
   *
   * @param val
   */
  void multiply(int val) {
    auto iter = _numStr.rbegin();
    int carry = 0;
    while (iter != _numStr.rend()) {
      int curr = (*iter - '0') * val;
      curr += carry;
      *iter = curr % 10 + '0';
      carry = curr / 10;
      ++iter;
    }
    if (carry != 0) {
      _numStr.insert(_numStr.begin(), carry + '0');
    }
  }

  /**
   * @brief Return the number of digits in this number.
   *
   * @return int
   */
  int size() const { return _numStr.size(); }

  bool operator<(Number const &b) const {
    if (_numStr.size() == 0 || b.size() == 0) {
      return _numStr.size() == 0 ? false : true;
    } else if (_numStr.size() < b.size()) {
      return true;
    } else if (_numStr.size() > b.size()) {
      return false;
    } else {
      auto iter1 = _numStr.begin();
      auto iter2 = b.getBegin();

      while (iter1 != _numStr.end() && iter2 != b.getEnd() && *iter1 == *iter2) {
        ++iter1;
        ++iter2;
      }

      return *iter1 < *iter2;
    }
  }

  // Friend function to output the number
  friend std::ostream &operator<<(std::ostream &out, const Number &number);

private:
  std::string::const_iterator getBegin() const { return _numStr.cbegin(); }

  std::string::const_iterator getEnd() const { return _numStr.cend(); }

  /**
   * Digits that represent a number, most significant bit is present at the
   * front. Representing each digit as a character instead of an int helps in
   * saving space for each digit in the number.
   *
   * @note Storage for a result of multiplication for a number
   * with a high number of digits might seem to naturally fit the usecase for an
   * std::list. However a list node in addition to the data in each node also
   * includes 2 pointers to the next and previous nodes which causes memory in
   * efficiency. In addition for the problem at hand, allocaiton of new nodes in
   * the list causes Random memory causes causing a significant degradation in
   * performance when compared to using a vector for the same number of digits.
   */
  std::string _numStr;
};

std::ostream &operator<<(std::ostream &out, const Number &number) {
  return out << number._numStr;
}

/**
 * @brief Shrink the size of the vector if the size is > MAX_SIZE
 *
 * @param multiples Vector of multiples of a prime number.
 * @param front Index of first unprocessed number in @c multiples. This value is
 * updated after the pruning operation.
 * @param maxSize Vector size at which point pruning will be applied.
 */
void pruneUsed(std::vector<int> &currIdxs,
               std::vector<std::vector<Number>> &multiples, long int maxSize) {
  for (int i = 0; i < currIdxs.size(); i++) {
    if (multiples[i].size() >= maxSize) {
      multiples[i].erase(multiples[i].begin(),
                         multiples[i].begin() + currIdxs[i]);
      multiples.shrink_to_fit();
      currIdxs[i] = 0;
    }
  }

  //// std::cout<< "Finished pruning " << std::endl;
}

std::pair<Number, int>
findMinCandidate(std::vector<int> const &currIdxs,
                 std::vector<std::vector<Number>> const &multiples) {
  int minIdx = 0;
  Number minNumber = Number();
  for (int i = 0; i < currIdxs.size(); i++) {
    //// std::cout<< currIdxs[i] << " " << multiples[i].size() << std::endl;
    Number const& nextCandidate = currIdxs[i] < multiples[i].size() ? multiples[i][currIdxs[i]] : Number();
    // std::cout<< "Next candidate " << nextCandidate << std::endl;
    if (nextCandidate < minNumber) {
      minIdx = i;
      minNumber = multiples[i][currIdxs[i]];
    }
  }

  return {minNumber, minIdx};
}

/**
 * @brief Generate the nth number that can be factored with given list
 * of prime numbers.
 *
 * @param n Nth number.
 * @param primes Vector of prime numbers.
 *
 * @return Nth Number.
 */
Number nthMultiple(long int n, std::vector<int> primes) {
  long int const MAX_SIZE = 1000000;
  unsigned int const numPrimes = primes.size();
  std::vector<int> currIdxs(numPrimes, 0);

  // TODO add more comments here as vector of multiples of each prime number.
  std::vector<std::vector<Number>> multiples(numPrimes, std::vector<Number>(1));

  for (int i = 0; i < primes.size(); i++) {
    multiples[i][0] = Number(primes[i]);
  }

  Number ans;
  Number maxSoFar(1);
  long int unprocessed = primes.size();
  for (long int i = 1;; i++) {
    auto next = findMinCandidate(currIdxs, multiples);
    Number nextNumber = next.first;
    int const nextPrimeNumberIdx = next.second;
    if (i == n - 1) {
      ans = nextNumber;
      break;
    }
    ++currIdxs[nextPrimeNumberIdx];
    // std::cout<< "Next number is " << nextNumber << " " << nextPrimeNumberIdx << std::endl;

    // One candidate is processed in this iteration.
    --unprocessed;

    // Numbers to be calculated to reach the target.
    long int const remaining = n - i;

    // In order to eliminate duplicate elements being added to the vectors, only
    // add multiples of this prime number and higher primes to their respective
    // vectors. Hence start checking if the current number is in the list of
    // primes in descending  order.
    //
    // For example if the current element is 5(i.e a multiple of 5), this
    // optimization prevents adding 10 and 15 to five's multiples which would
    // have been already been added when elements 2 (2*5 = 10) and 3 (3*5 = 15)
    // were previously processed. In order to prevent multiplying elements that
    // will not be candidates try to multiply with the highest prime number
    // going down to the index of the vector from which the next candidate is
    // picked from.
    int primeIdx = nextPrimeNumberIdx;
    while (primeIdx < primes.size()) {
      Number nextMultiple = nextNumber;
      nextMultiple.multiply(primes[primeIdx]);
      // std::cout<< "unprocessed " << unprocessed << " to " << remaining << " remaining -- max " << maxSoFar << " " << nextMultiple << std::endl;
      // If the number of unprocessed numbers is greater than the remaining
      // numbers to be processed to get to the target, then only add the newly
      // calculated number if it is greater than the max element that is added
      // to all lists at any point in time.
      if (unprocessed >= remaining && maxSoFar < nextMultiple) {
        break;
      }

      maxSoFar = std::max(maxSoFar, nextMultiple);
      // std::cout<< "Adding " << nextMultiple << " to " << primeIdx << " max " << maxSoFar << std::endl;
      
      multiples[primeIdx].emplace_back(nextMultiple);
      ++unprocessed;
      ++primeIdx;
    }

    pruneUsed(currIdxs, multiples, MAX_SIZE);
  }

  return ans;
}

// Driver code
int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Invalid number of arguments " << std::endl;
    return -1;
  }
  std::cout<< nthMultiple(strtol(argv[1], NULL, 10), {2, 3, 5}) << std::endl;

  return 0;
}