#ifndef CALC_PRIME_H
#define CALC_PRIME_H

#include <vector>
#include <ctime>

bool isProbablePrime(int n);
int jacobi(int a, int n);
bool millerRabin(int n, int b = 2);
int pollardRho(int n);

std::vector<int> getPrimeFactors(int n);

#endif //CALC_PRIME_H
