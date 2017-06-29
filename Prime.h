#ifndef CALC_PRIME_H
#define CALC_PRIME_H

/**
 * Utility functions for prime factorization.
 */

#include <vector>

/**
 * @brief Determines if \a n is a prime number.
 *
 * Determines if \a n is a prime number using a modified Baillie-PSW primality test.
 * This modified test uses base 2 and base 3 Miller-Rabin primality tests and the strong Lucas probable prime test.
 *
 * @see <a href="https://en.wikipedia.org/wiki/Baillie%E2%80%93PSW_primality_test">Baillie-PSW primality test</a>
 * @see millerRabin
 * @see lucasPP
 *
 * @bug lucasPP is misbehaving, currently this test does not include the strong Lucas test.
 *
 * @param n the number to be tested.
 * @return true if \a n is prime.
 */
bool isProbablePrime(int n);

/**
 * Strong Lucas probable prime test.
 *
 * @see <a href="https://en.wikipedia.org/wiki/Lucas_pseudoprime">Lucas probable prime test</a>
 *
 * @param n
 * @return
 */
bool lucasPP(int n);

/**
 * Miller-Rabin primality test.
 *
 * @see <a href="https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test">Miller-Rabin primality test</a>
 *
 * @param n Number to test
 * @param b Base
 * @return
 */
bool millerRabin(int n, int b);

/**
 * Base-2 millerRabin test.
 *
 * @see millerRabin(n, b)
 *
 * @param n Number to test
 * @return
 */
bool millerRabin(int n);

/**
 * Find a prime factor of \a n by using Pollard's rho algorithm.
 *
 * @see <a href="https://en.wikipedia.org/wiki/Pollard%27s_rho_algorithm">Pollard's rho algorithm</a>
 *
 * @param n
 * @return A prime factor of \a n.
 */
int pollardRho(int n);

/**
 * Finds all prime factors of \a n.
 *
 * @see pollardRho
 *
 * @param n
 * @return A vector<int> containing all prime factors of \a n.
 */
std::vector<int> getPrimeFactors(int n);

#endif //CALC_PRIME_H
