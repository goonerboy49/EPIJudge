#include <iostream>
#include <list>
#include <numeric>
#include <queue>
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
  Number(int val) { _nums.push_front(val); }

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
      _nums.push_front(carry);
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
  bool operator>(Number const &b) const {
    if (_nums.size() < b.size()) {
      return false;
    } else if (_nums.size() > b.size()) {
      return true;
    } else {
      auto iter1 = _nums.begin();
      auto iter2 = b.getBegin();

      while (iter1 != _nums.end() && iter2 != b.getEnd() && *iter1 == *iter2) {
        ++iter1;
        ++iter2;
      }

      return *iter1 > *iter2;
    }
  }

  friend std::ostream &operator<<(std::ostream &out, const Number &number);

private:
  std::list<int>::const_iterator getBegin() const { return _nums.cbegin(); }

  std::list<int>::const_iterator getEnd() const { return _nums.cend(); }

  // Digits that represent a number, most significant bit is present at the
  // front. Storing individual digits as a node in the list helps in
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

using NumberHeap =
    std::priority_queue<Number, std::vector<Number>, std::greater<Number>>;

/**
 * @brief Update the heap with next set of prospective elements given a number
 * that is removed from the heap. Also update the maxSoFar number if the new
 * number added to the heap is higher than maxSoFar.
 *
 * @param currNumber Current number popped from the heap
 * @param currIdx Idx of current number
 * @param n Nth number to be found
 * @param primes List of sorted primes
 * @param minHeap Referece to the heap of future elements.
 * @param maxSoFar Reference to the maximum element present in the heap.
 */
void UpdateHeap(Number const &currNumber, int currIdx, int n,
                std::vector<int> const &primes, NumberHeap &minHeap,
                Number &maxSoFar) {
  auto iter = primes.end();

  // In order to eliminate duplicate elements being added to the heap, only add
  // multiples of this prime number and higher primes to the heap. Hence start
  // checking if the current number is in the list of primes in descending
  // order.
  //
  // For example if the current element is 5, this optimization prevents adding
  // 10 and 15 to the heap which would have been already been added when
  // elements 2 (2*5 = 10) and 3 (3*5 = 15) are popped from the heap.
  while (iter != primes.begin()) {
    --iter;
    if (currNumber.isMultiple(*iter)) {
      while (iter != primes.end()) {
        Number multiple = currNumber;
        multiple.multiply(*iter);

        // If the remaining elements in the heap is greater than n-currIdx then
        // add the new element only when it is less than the max element seen so
        // far so that items that will not be useful to the answer are not added
        // to the heap.
        if (minHeap.size() < n - currIdx - 1 || maxSoFar > multiple) {
          minHeap.emplace(multiple);
        }

        ++iter;
        maxSoFar = iter == primes.end() ? multiple : maxSoFar;
      }
      break;
    }
  }
}

/**
 * @brief Generate the nth number that can be factored with given list
 * of prime numbers.
 *
 * @param n Nth number.
 * @param primes Sorted list of prime numbers.
 * @return Nth Number.
 */
Number nthMultiple(long int n, std::vector<int> primes) {
  // Sort in ascending order.
  NumberHeap minHeap;

  for (int i = 0; i < primes.size(); i++) {
    Number number(primes[i]);
    minHeap.emplace(number);
  }

  Number maxSoFar(primes.back());

  for (int i = 1; i < n - 1; i++) {
    Number top = minHeap.top();
    minHeap.pop();

    UpdateHeap(top, i, n, primes, minHeap, maxSoFar);
  }
  return minHeap.top();
}
// Driver code
int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Invalid number of arguments " << std::endl;
    return -1;
  }
  std::cout << nthMultiple(strtol(argv[1], NULL, 10), {2, 3, 5}) << std::endl;

  return 0;
}