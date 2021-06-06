#include <iostream>
#include <list>
#include <numeric>
#include <vector>

/**
 * Algorithm
 *
 * The algorithm implemented here is Dynamic programming with prime multiples
 * memoized as candidates for the next least number factorable by the given
 * prime numbers. The algorithm picks the least number from a pool of candidates
 * generated from previous numbers in the sequence generated to get to n. For
 * instance if the next number in the sequence is x then {x*prime[0],
 * x*prime[1]...} are generated and stored in the candidates list. This
 * operation is run n times to reach the nth number and in each iteration the
 * least number from the generated candidates is picked.
 *
 * Data structures:
 *
 * - std::vector<char> to represent a number with digits that cannot be stored
 * in in-built types.
 *    -A vector can be iterated for the  multiplicatiion
 * operation to generate prime multiples of the next least candidate chosen.
 *    -It also provides ability to increase the size as the number of digits
 * grow. -Lastly as compared to a std::list in a std::vector memory is
 * contiguously allocated and hence delivers higher performance when iterating
 * over the digits during the multiplication operation. This was evidenced
 * especially at higher values of n when vector significantly outperformed a
 * list.
 *
 * -std::vector<Number> to store the generated candidate multiples factorable by
 * the given prime numbers. A std::priority_queue(heap) could also be used to
 * store the next multiples which provides ability to look up the next least
 * candidate in constant time, but at higher values of n it was observed that a
 * std::vector performed better than the heap as each value inserted and deleted
 * from the heap results in a log(heapSize) run time complexity
 *
 * Optimizations:
 * - Use a separate multiples vector for each prime number and store the
 * multiples in it's vector in order to avoid generating and storing duplicates.
 * See below in the code for an example.
 * - Pre allocate the multiples vector size based on the requested value of n.
 * Higher the value of n the higher the pre reserved value of multiples vector.
 * Expanding the vector when adding new elements causes re allocation of
 * existing elements and using the pre reserved vector the code tries to
 * optimize the insertion of newly generated candidate elements.
 * - Store the number in reverse i.e most significant digit stored at the back
 * of the vector so that the vector can be resized and any new elements are
 * added to the end in constant time without incurring the penalty of moving
 * existing elements as compared to inserting at the front.
 * - Store the digits are characters instead of integer to save on storage
 * space.
 * - Prune used elements in the memoized multiples vector to reclaim space for
 * newer multiples that are generated.
 *
 *
 * Complexity:
 *
 * m - Number of prime numbers (In the originally stated problem where prime
 * numbers are {2,3,5} = 3)
 * n - Nth number requested
 *
 *  -Run time complexity:
 *    O(n * m)
 *    The algorithm runs n iterations and in each iteration upto m new candidate
 * multiples are inserted to their respective vectors. Each insertion is
 * performed to the end of the vector and is done in constant time.
 *
 *    For higher values of n the pruning optimization described above results in
 * erasing a large portions of the vector containing used elements and resizing
 * the vector which adds to run time complexity(See limitations)
 *
 *  -Space complexity:
 *  O(m * getMaxReservation(n))
 *  A pre reserved vector is used to store the candidate multiples. The size of
 * the pre reserved vector for each prime number depends on the value of n. See
 * @c getMaxReservation below.
 *
 * Limitations:
 *
 * - Although this algorithm performs well at reasonable values of n
 *
 *
 * Runtimes:
 * On Ubuntu VM with 4 cores and 6 GB main memory
 *
 * time build/multiples 1500
859963392
build/multiples 1500  0.00s user 0.00s system 56% cpu 0.006 total
 *
 * time build/multiples 1000000
519312780448388736089589843750000000000000000000000000000000000000000000000000000000
build/multiples 1000000  0.40s user 0.08s system 99% cpu 0.480 total
 *
 * time build/multiples 10000000
162441050638304318232392153117595750351085388205966408633356724833252116013682098127
901554107666015625000000000000000000000000000000000000000000000000000000000000000000
00000000000000
build/multiples 10000000  5.41s user 1.25s system 99% cpu 6.683 total
 *
 * time build/multiples 100000000
181401433096113635329533424306933545846696350337090979294625053667140351565931358183
804678660542229646351449148549495502713754427213681221919720410943110751075070675731
471915021942015682682026147816946818595136490836162942005416114894699679995595053651
728120955680200739341006998503970330059031581136915184569121499899196013858752270494
01605594538145621585911726469930727034807205200195312500
build/multiples 100000000  99.17s user 3.44s system 98% cpu 1:44.27 total
 */

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
  Number(int val) {
    _numStr.reserve(1);
    _numStr.push_back(char('0' + val));
  }
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
    auto iter = _numStr.begin();
    int carry = 0;
    while (iter != _numStr.end()) {
      int curr = (*iter - '0') * val;
      curr += carry;
      *iter = curr % 10 + '0';
      carry = curr / 10;
      ++iter;
    }
    if (carry != 0) {
      // Use reserve here instead of resize as resize grows the vector size by
      // double.
      _numStr.reserve(_numStr.capacity() + 1);
      _numStr.push_back(char(carry + '0'));
    }
  }

  /**
   * @brief Return the number of digits in this number.
   *
   * @return int
   */
  int size() const { return _numStr.size(); }

  bool operator<(Number const &b) const {
    // Handle an uninitialized Number with no digits
    if (_numStr.size() == 0 || b.size() == 0) {
      return _numStr.size() == 0 ? false : true;
    } else if (_numStr.size() < b.size()) {
      return true;
    } else if (_numStr.size() > b.size()) {
      return false;
    } else {
      auto iter1 = _numStr.crbegin();
      auto iter2 = b.getRbegin();

      while (iter1 != _numStr.crend() && iter2 != b.getRend() &&
             *iter1 == *iter2) {
        ++iter1;
        ++iter2;
      }

      return *iter1 < *iter2;
    }
  }

  // Friend function to output the number
  friend std::ostream &operator<<(std::ostream &out, const Number &number);

private:
  std::vector<char>::const_reverse_iterator getRbegin() const {
    return _numStr.crbegin();
  }

  std::vector<char>::const_reverse_iterator getRend() const {
    return _numStr.crend();
  }

  /**
   * Digits that represent a number
   *
   * Optimizations -
   * - Most significant digit in the number is present at the back of the
   * vector, storing the number reversed makes storing any new digits from a
   * multiply operation at the back of the vector and hence preventing a costly
   * reallocation of existing elements when the new digit result is added to the
   * front of the vector.
   *
   * - Representing each digit as a character instead of an saves storage space
   * required to store the number.
   *
   * @note Storage for a result of multiplication for a number
   * with a high number of digits might seem to naturally fit the usecase for an
   * std::list. However a list node in addition to the data in each node also
   * includes 2 pointers to the next and previous nodes which causes memory in
   * efficiency. In addition for the problem at hand, allocaiton of new nodes in
   * the list causes Random memory causes causing a significant degradation in
   * performance when compared to using a vector for the same number of digits.
   */
  std::vector<char> _numStr;
};

long int getMaxReservation(long int n);
std::pair<Number, int>
findMinCandidate(std::vector<long int> const &nextCandidateIdxs,
                 std::vector<long int> const &nextInsertionIdxs,
                 std::vector<std::vector<Number>> &multiples);
std::pair<long int, long int> insert(std::vector<Number> &multiples,
                                     long int nextInsertionIdx,
                                     long int nextCandidateIdx,
                                     Number nextMultiple);

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
  /*
   * Approach
   *
   * - - Reserve the individual vectors based on the value n. A basic
   * reservation algorithm is implemented here, but should be tuned to the
   * target. Reserving the vector size prevents costly re allocations when new
   * elements are added to the vector.
   * - Maintain a separate vector of Number objects for each prime number. These
   * are the candidate Numbers for finding the next least number that is
   * factorable only from the prime numbers.
   * - Find the least number from the first non processed candidate of each
   * vector.
   * - Generate multiples of primes higher than the vector from which the
   * current element was chosen and append it to their respective vectors. This
   * helps in eliminating duplicates in each of the vectors.
   *
   * For instance for primes {2, 3, 5}
   * Initial state
   * multiples[0] - {2}
   * multiples[1] - {3}
   * multiples[2] - {5}
   * nextCandidateIdxs {0,0,0}
   *
   * min(multiples[0][nextCandidateIdx[0]], .. ) is min(2,3,5) = 2
   *
   * multiples[0] - {2, 4}
   * multiples[1] - {3, 6}
   * multiples[2] - {5, 10}
   * nextCandidateIdxs {1,0,0}
   *
   * min(multiples[0][nextCandidateIdx[0]], .. ) is min(4,3,5) = 3
   *
   * multiples[0] - {2, 4}
   * multiples[1] - {3, 6, 9}
   * multiples[2] - {5, 10, 15}
   * nextCandidateIdxs {1,1,0}
   *
   * min(multiples[0][nextCandidateIdx[0]], .. ) is min(4,6,5) = 5
   */

  long int const maxReserved = getMaxReservation(n);
  unsigned int const numPrimes = primes.size();

  // 2-D vector to store the generated multiples of each prime number. The
  // vectors are sorted as each new multiple is added to the back of the vector.
  std::vector<std::vector<Number>> multiples(numPrimes,
                                             std::vector<Number>(maxReserved));
  std::vector<long int> nextCandidateIdxs(numPrimes, 0);
  std::vector<long int> nextInsertionIdxs(numPrimes, 1);

  for (int i = 0; i < primes.size(); i++) {
    multiples[i][0] = Number(primes[i]);
  }

  Number ans(1);
  Number maxSoFar(1);

  // Count of unprocessed candidates.
  long int unprocessed = primes.size();
  for (long int i = 1; i < n; i++) {
    auto next =
        findMinCandidate(nextCandidateIdxs, nextInsertionIdxs, multiples);
    Number nextNumber = next.first;
    int const nextPrimeNumberIdx = next.second;
    if (i == n - 1) {
      ans = nextNumber;
      break;
    }
    // Increment the index for the next candidate.
    ++nextCandidateIdxs[nextPrimeNumberIdx];

    // One candidate is processed in this iteration.
    --unprocessed;

    // Numbers to be calculated to reach the target.
    long int const remaining = n - i;

    // In order to eliminate duplicate elements being added to the vectors, only
    // add next least number multiplied by higher primes to their respective
    // vectors.
    //
    // For example if the current element is 10(i.e a multiple of 5), this
    // optimization prevents adding 20 and 30 to five's multiples which would
    // have been already been added when elements 4 (4*5 = 20) and 6 (6*5 = 30)
    // were previously processed.
    int primeIdx = nextPrimeNumberIdx;
    while (primeIdx < primes.size()) {
      Number nextMultiple = nextNumber;
      nextMultiple.multiply(primes[primeIdx]);
      // Optimization - If the number of unprocessed numbers is greater than the
      // remaining numbers to be processed to get to the target, then only add
      // the newly calculated number if it is lesser than the max element that
      // is added to all lists at any point in time.
      if (unprocessed >= remaining && maxSoFar < nextMultiple) {
        break;
      }

      maxSoFar = std::max(maxSoFar, nextMultiple);
      auto nextIdxs = insert(multiples[primeIdx], nextInsertionIdxs[primeIdx],
                             nextCandidateIdxs[primeIdx], nextMultiple);
      nextInsertionIdxs[primeIdx] = nextIdxs.first;
      nextCandidateIdxs[primeIdx] = nextIdxs.second;
      ++unprocessed;
      ++primeIdx;
    }
  }

  return ans;
}

/**
 * @brief Get the min number from the next candidates in multiples vectors
 *
 * @param nextCandidateIdxs Vector of indexes for the next candidates in each
 * prime multiple vector.
 * @param nextInsertionIdxs Vector of indexes for the next insertion position in
 * each prime multiple vector.
 * @param multiples 2D vector of prime multiples.
 *
 * @return std::pair<Number, int> Returns the next minimum multiple and the
 * index of the vector it was picked from.
 */
std::pair<Number, int>
findMinCandidate(std::vector<long int> const &nextCandidateIdxs,
                 std::vector<long int> const &nextInsertionIdxs,
                 std::vector<std::vector<Number>> &multiples) {
  int minIdx = 0;
  Number minNumber = Number();
  for (int i = 0; i < nextCandidateIdxs.size(); i++) {
    // If there are no more candidates in this vector use an empty number.
    Number const &nextCandidate = nextCandidateIdxs[i] < nextInsertionIdxs[i]
                                      ? multiples[i][nextCandidateIdxs[i]]
                                      : Number();
    if (nextCandidate < minNumber) {
      minIdx = i;
      minNumber = multiples[i][nextCandidateIdxs[i]];
    }
  }

  return {minNumber, minIdx};
}

/**
 * @brief Insert the next multiple in the given @c multiples vector at @c
 * nextInsertionIdx position. This method shrinks or grows the vector based on
 * the avaialble unused capacity.
 *
 * @param multiples Multiples vector.
 * @param nextInsertionIdx Postion of the next insertion in @c multiples.
 * @param nextCandidateIdx Postion of the next min unprocessed element in @c
 * multiples.
 * @param nextMultiple Next multiple to insert.
 * @return std::pair<long int, long int> Returns the updated values of @c
 * nextInsertionIdx and @c nextCandidateIdx
 */
std::pair<long int, long int> insert(std::vector<Number> &multiples,
                                     long int nextInsertionIdx,
                                     long int nextCandidateIdx,
                                     Number nextMultiple) {
  // If the capacity of the vector is reached
  //
  // - Try to erase elements from the front that were already processed. Since
  // an erase operaion results in copy and movement of existing elements in the
  // vector, an optimization is used here such that used elements are erased
  // only if the number of used elements exceeds 50% of vector capacity.
  // - If the elements cannot be erased then resize the vector with double the
  // current reservation.
  if (nextInsertionIdx == multiples.capacity()) {
    long int newReservation = multiples.capacity();
    if (nextCandidateIdx > (multiples.capacity() / 2)) {
      multiples.erase(multiples.begin(),
                      (multiples.begin() + nextCandidateIdx));
      // Release the used memory.
      multiples.shrink_to_fit();

      // Update the next insertion position and next candidate positions.
      nextInsertionIdx -= nextCandidateIdx;
      nextCandidateIdx = 0;
    } else {
      newReservation *= 2;
    }
    multiples.resize(newReservation);
  }

  // Append the new multiple and increment the insertion position.
  multiples[nextInsertionIdx] = nextMultiple;
  ++nextInsertionIdx;

  return {nextInsertionIdx, nextCandidateIdx};
}

/**
 * @brief Get the Max Reservation size for a given n. This method returns a
 * tiered reservation value such that prime multiples vector is pre reserved
 * based on the value of n.
 *
 * @param n
 * @return long int
 */
long int getMaxReservation(long int n) {
  if (n < 1000) {
    return 10;
  } else if (n < 10000) {
    return 100;
  } else if (n < 1000000) {
    return 1000;
  } else if (n < 1000000) {
    return 10000;
  } else if (n < 10000000) {
    return 1000000;
  } else if (n < 100000000) {
    return 10000000;
  } else {
    return 10000000;
  }
}

std::ostream &operator<<(std::ostream &out, const Number &number) {
  // Digits are stored in reverse order, hence traverse from the end of the
  // vector.
  auto iter = number.getRbegin();
  while (iter != number.getRend()) {
    out << *iter;
    ++iter;
  }
  return out;
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