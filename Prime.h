#ifndef CALC_PRIME_H
#define CALC_PRIME_H

#include <vector>
#include <ctime>

bool isProbablePrime(int n);
int jacobi(int a, int n);
int findD(int n);
bool lucas_pp(int n, int D, int P, int Q);
bool millerRabin(int n, int b = 2);
int pollardRho(int n);

std::vector<int> getPrimeFactors(int n);

#endif //CALC_PRIME_H
