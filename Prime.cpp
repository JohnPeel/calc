#include <bitset>
#include <cmath>
#include <iostream>
#include <utility>
#include <cassert>
#include "Prime.h"
#include "Utility.h"

bool isProbablePrime(uint64_t n) {
    if ((n == 0) or (n == 1))
        return false;

    for (int known_prime : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47})
        if (n == known_prime)
            return true;
        else if (n % known_prime == 0)
            return false;

    if ((not millerRabin(n, 2)) or (not millerRabin(n, 3)))
        return false;

    double sqrtn = std::sqrt(n);
    if (sqrtn == round(sqrtn))
        return false;

    int D = findD(n);
    if (not lucasPP(n, D, 1, (1 - D) / 4))
        return false;

    {}  // NOTE: Irrelevant, but keep it. (CLion syntax warning)
    return true;
}

std::vector<bool> getBits(uint64_t k) {
    std::vector<bool> bits;
    do bits.push_back((bool)(k & 1)); while (k >>= 1);
    std::reverse(bits.begin(), bits.end());
    return bits;
}

void lucasSeq(int64_t& U, int64_t& V, uint64_t k, uint64_t n, int32_t P, int32_t Q, int32_t D) {
    U = 1; V = P;
    int s = 1;
    std::vector<bool> bits = getBits(k);
    bits.erase(bits.begin());

    for (bool bit : bits) {
        U = mod<int64_t>(U * V, n);
        V = mod<int64_t>(pow_mod<int64_t>(V, 2, n) - 2 * pow_mod<int64_t>(Q, s, n), n);
        s *= 2;

        if (bit) {
            int64_t newU = P * U + V, newV = D * U + P * V;
            if (newU % 2 != 0)
                newU += n;
            if (newV % 2 != 0)
                newV += n;

            U = mod<int64_t>(newU / 2, n);
            V = mod<int64_t>(newV / 2, n);
            s += 1;
        }
    }
}

bool lucasPP(uint64_t n, int32_t D, int32_t P, int32_t Q) {
    assert(std::__gcd<int64_t>(n, D) == 1);
    assert(jacobi(D, n) == -1);

    int64_t U = 1, V = P;
    uint64_t k = n - jacobi(D, n);

    lucasSeq(U, V, k, n, P, Q, D);

    if (U != 0)
        return false;

    int32_t s = 0;
    while (k % 2 == 0) {
        k /= 2;
        s += 1;
    }

    lucasSeq(U, V, k, n, P, Q, D);

    if ((U == 0) or (V == 0))
        return true;

    for (int32_t r = 1; r < s; r++) {
        V = mod<int64_t>(pow_mod<int64_t>(V, 2, n) - 2 * pow_mod<int64_t>(Q, k, n), n);
        k *= 2;

        if (V == 0)
            return true;
    }

    assert(k = n - D);

    return false;
}

int32_t jacobi(int64_t a, uint64_t n) {
    if (n == 1)
        return 1;

    a %= n; // Rule 2

    if ((a == 0) or (a == 1))
        return (int)a;

    // Rule 3
    if (std::__gcd<int64_t>(a, n) != 1)
        return 0;

    // Rule 8
    if (a == 2)
        switch (n % 8) {
            case 1: case 7:
                return 1;
            case 3: case 5:
                return -1;
            default: {} // NOTE: Irrelevant, but keep it. (CLion syntax warning)
        }

    // Rule 7
    if (a == -1)
        return (n % 4 == 1) ? 1 : -1;

    // Rule 4
    if (a < 0)
        return jacobi(-1, n) * jacobi(-a, n);
    if (a % 2 == 0)
        return jacobi(2, n) * jacobi(a / 2, n);

    assert(a > 0);
    assert(n % 2 != 0);

    // Rule 6
    if ((a % 4 == 3) and (n % 4 == 3))
        return -jacobi(n, a);
    else
        return jacobi(n, a);
}

int32_t findD(int64_t n) {
    int32_t D = 5;
    while (jacobi(D, n) != -1) {
        D += (D < 0) ? -2 : 2;
        D *= -1;
    }
    return D;
}

bool millerRabin(int64_t n, int64_t b) {
    int64_t d = n - 1;
    int32_t r = 0;

    while (d % 2 == 0) {
        d /= 2;
        r += 1;
    }

    int64_t x = pow_mod(b, d, n);

    if ((x == 1) or (x == n - 1))
         return true;

    for (int32_t i = 0; i < r - 1; i++) {
        x = pow_mod<int64_t>(x, 2, n);
        if (x == 1)
            return false;
        if (x == n - 1)
            return true;
    }

    return false;
}

int64_t _pollardRho(int64_t n, int64_t x, int64_t c) {
    int64_t y = x;
    int64_t factor = 1;
    int64_t cycle_size = 2;

    while (factor == 1) {
        for (int64_t count = 1; count <= cycle_size && factor <= 1; count++) {
            x = pow_mod<int64_t>(x, 2, n) + c % n;
            factor = std::__gcd<int64_t>(abs(x - y), n);
        }

        if (factor == n)
            return _pollardRho(n, (rand() % (n - 2)) + 2, (rand() % (n - 1)) + 1);

        cycle_size *= 2;
        y = x;
    }

    return factor;
}

int64_t pollardRho(int64_t n) {
    std::srand((unsigned int)std::time(NULL));

    if (n == 1)
        return 1;

    if (isProbablePrime(n))
        return n;

    if (not n & 1)
        return 2;

    return _pollardRho(n, 2, 1);
}

std::vector<int> getPrimeFactors(int n) {
    std::vector<int> ret;

    if (n < 0) {
        ret.push_back(-1);
        n *= -1;
    }

    int m = 1;

    while (n / m != 1) {
        int factor = (int)pollardRho(n / m);
        ret.push_back(factor);
        m *= factor;
    }

    return ret;
}
