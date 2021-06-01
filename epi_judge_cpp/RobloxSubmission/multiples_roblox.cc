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

  Number &operator=(Number &&other) {
    if (this != &other) {
      _nums = other._nums;
    }

    return *this;
  }

  Number(Number &&other) { _nums = other._nums; }

  Number(const Number &other) {
    std::copy(other._nums.begin(), other._nums.end(),
              std::back_inserter(_nums));
  }

  Number &operator=(const Number &other) {
    if (this != &other) {
      std::vector<int>().swap(_nums);
      std::copy(other._nums.begin(), other._nums.end(),
                std::back_inserter(_nums));
    }
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

  void clear() { _nums.clear();}

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
    if (_nums.size() < b.size()) {
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

std::pair<int, int> getNextCandidatePos(std::vector<std::vector<Number>>& multiples, std::pair<int, int> const& currPos, int MAX_SIZE) {
  int x = currPos.first;
  int y = currPos.second;

  Number curr = multiples[x][y];
  multiples[x][y].clear();
  if (y == MAX_SIZE-1) {
    y = 0;
  } else {
    y++;
  }

  Number const& next = multiples[x][y];
  if ((multiples.size() == 1 && next.size() == 0) || (next.size() > 0 && curr < next)) {
    return {x,y};
  }

  multiples.erase(multiples.begin(), multiples.begin()+1);
  return {0,0};
}

std::pair<int, int> getNextInsertionPos(std::vector<std::vector<Number>>& multiples, std::pair<int, int> const& currInsPos, int MAX_SIZE) {
  int x = currInsPos.first;
  int y = currInsPos.second;
  if (multiples.size() == x) {
    --x;
  }

  Number const& curr = multiples[x][y];
  if (y == MAX_SIZE-1) {
    y = 0;
  } else {
    ++y;
  }

  Number const& next = multiples[x][y];
  if (next.size() == 0) {
    return {x,y};
  }

  multiples.emplace_back(std::vector<Number>(MAX_SIZE));
  return {++x, 0};
}

/**
 * @brief Generate the nth number that can be factored with given list
 * of prime numbers.
 *
 * @param n Nth number.
 * @return Nth Number.
 */
Number nthMultiple(long int n) {
  int MAX_SIZE=10000;
  std::vector<std::vector<Number>> twoMultiples;
  twoMultiples.emplace_back(std::vector<Number>(MAX_SIZE));
  twoMultiples[0][0] = Number(2);

  std::vector<std::vector<Number>> threeMultiples;
  threeMultiples.emplace_back(std::vector<Number>(MAX_SIZE));
  threeMultiples[0][0] = Number(3);

  std::vector<std::vector<Number>> fiveMultiples;
  fiveMultiples.emplace_back(std::vector<Number>(MAX_SIZE));
  fiveMultiples[0][0] = Number(5);

  std::pair<int, int> nextCandidate2(0,0);
  std::pair<int, int> nextCandidate3(0,0);
  std::pair<int, int> nextCandidate5(0,0);  

  std::pair<int, int> insertionPos2(0,0);
  std::pair<int, int> insertionPos3(0,0);
  std::pair<int, int> insertionPos5(0,0);

  Number ans;
  int count5=0;
  for (int i = 1;; i++) {
    // std::cout << "Next Candidate 5[" << nextCandidate5.first << "][" << nextCandidate5.second << "] " << fiveMultiples[nextCandidate5.first][nextCandidate5.second] << std::endl;
    // std::cout << "Next Candidate 3[" << nextCandidate3.first << "][" << nextCandidate3.second << "] " << threeMultiples[nextCandidate3.first][nextCandidate3.second] << std::endl;
    // std::cout << "Next Candidate 2[" << nextCandidate2.first << "][" << nextCandidate2.second << "] " << twoMultiples[nextCandidate2.first][nextCandidate2.second] << std::endl;
    Number nextNumber = std::min(std::min(twoMultiples[nextCandidate2.first][nextCandidate2.second], threeMultiples[nextCandidate3.first][nextCandidate3.second]),
                                 fiveMultiples[nextCandidate5.first][nextCandidate5.second]);
    if (i == n - 1) {
      ans = nextNumber;
      return nextNumber;
    }
    // std::cout << "++Next number is " << nextNumber << std::endl;

    if (nextNumber.isMultiple(5)) {
      Number fiveMultiple = nextNumber;
      fiveMultiple.multiply(5);
      
      nextCandidate5 = getNextCandidatePos(fiveMultiples, nextCandidate5, MAX_SIZE);
      insertionPos5 = getNextInsertionPos(fiveMultiples, insertionPos5, MAX_SIZE);
      fiveMultiples[insertionPos5.first][insertionPos5.second] = fiveMultiple;
    } else if (nextNumber.isMultiple(3)) {
      Number fiveMultiple = nextNumber;
      fiveMultiple.multiply(5);

      insertionPos5 = getNextInsertionPos(fiveMultiples, insertionPos5, MAX_SIZE);
      fiveMultiples[insertionPos5.first][insertionPos5.second] = fiveMultiple;

      Number threeMultiple = nextNumber;
      threeMultiple.multiply(3);

      nextCandidate3 = getNextCandidatePos(threeMultiples, nextCandidate3, MAX_SIZE);
      insertionPos3 = getNextInsertionPos(threeMultiples, insertionPos3, MAX_SIZE);
      threeMultiples[insertionPos3.first][insertionPos3.second] = threeMultiple;
    } else {
      Number fiveMultiple = nextNumber;
      fiveMultiple.multiply(5);
      
      
      insertionPos5 = getNextInsertionPos(fiveMultiples, insertionPos5, MAX_SIZE);
      fiveMultiples[insertionPos5.first][insertionPos5.second] = fiveMultiple;

      Number threeMultiple = nextNumber;
      threeMultiple.multiply(3);
      insertionPos3 = getNextInsertionPos(threeMultiples, insertionPos3, MAX_SIZE);
      threeMultiples[insertionPos3.first][insertionPos3.second] = threeMultiple;

      Number twoMultiple = nextNumber;
      twoMultiple.multiply(2);

      nextCandidate2 = getNextCandidatePos(twoMultiples, nextCandidate2, MAX_SIZE);
      insertionPos2 = getNextInsertionPos(twoMultiples, insertionPos2, MAX_SIZE);
      twoMultiples[insertionPos2.first][insertionPos2.second] = twoMultiple;
    }

    // std::cout << "Next Insertion 5[" << insertionPos5.first << "][" << insertionPos5.second << "] " << std::endl;
    // std::cout << "Next Insertion 3[" << insertionPos3.first << "][" << insertionPos3.second << "] " << std::endl;
    // std::cout << "Next Insertion 2[" << insertionPos2.first << "][" << insertionPos2.second << "] " << std::endl;
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