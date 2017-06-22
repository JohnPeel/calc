#ifndef CALC_PRIME_H
#define CALC_PRIME_H

#include <vector>
#include <ctime>

bool isProbablePrime(uint64_t n);
int32_t jacobi(int64_t a, uint64_t n);
int32_t findD(int64_t n);
void lucasSeq(int64_t& U, int64_t& V, uint64_t k, uint64_t n, int32_t P, int32_t Q, int32_t D);
bool lucasPP(uint64_t n, int32_t D, int32_t P, int32_t Q);
bool millerRabin(int64_t n, int64_t b);
int64_t pollardRho(int64_t n);

std::vector<int> getPrimeFactors(int n);

#endif //CALC_PRIME_H
