#include <bitset>
#include <cmath>
#include <iostream>
#include <utility>
#include <cassert>
#include "Prime.h"
#include "Utility.h"

bool isProbablePrime(int n) {
    if ((n == 0) or (n == 1))
        return false;

    for (int known_prime : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47})
        if (n == known_prime)
            return true;
        else if (n % known_prime == 0)
            return false;

    if (not millerRabin(n))
        return false;

    double sqrtn = std::sqrt(n);
    if (sqrtn == round(sqrtn))
        return false;

    int D = findD(n);
    if (not lucas_pp(n, D, 1, (1 - D) / 4))
        return false;

    {} //CLion likes to whine...
    return true;
}

std::vector<bool> getBits(int k) {
    std::vector<bool> bits;
    do bits.push_back((bool)(k & 1)); while (k >>= 1);
    std::reverse(bits.begin(), bits.end());
    return bits;
}

void lucas_seq(int& U, int& V, int k, int n, int P, int Q, int D) {
    int s = 1;
    std::vector<bool> bits = getBits(k);
    bits.erase(bits.begin());

    for (bool bit : bits) {
        U = (U * V) % n;
        V = (modpow(V, 2, n) - 2 * modpow(Q, s, n)) % n;
        s *= 2;

        if (bit) {
            int newU, newV;

            if ((P * U + V) % 2 == 0)
                newU = ((P * U + V) / 2) % n;
            else
                newU = ((P * U + V + n) / 2) % n;

            if ((D * U + P * V) % 2 == 0)
                newV = ((D * U + P * V) / 2) % n;
            else
                newV = ((D * U + P * V + n) / 2) % n;

            s += 1;
            U = newU;
            V = newV;
        }
    }
}

bool lucas_pp(int n, int D, int P, int Q) {
    assert(std::__gcd(n, D) == 1);

    int U = 1, V = P, k = n + 1;

    lucas_seq(U, V, k, n, P, Q, D);
    std::cout << "lucas_seq(U = 1, V = " << P << ", k = " << k << ", n = " << n << ", P = " << P << ", Q = " << Q << ", D = " << D << ") -> U = " << U << ", V = " << V << std::endl;

    if (U != 0)
        return false;

    int s = 0;

    while (k % 2 == 0) {
        k /= 2;
        s += 1;
    }

    U = 1;
    V = P;
    lucas_seq(U, V, k, n, P, Q, D);

    if (U == 0)
        return true;

    for (int r = 1; r < s; r++) {
        U = (U * V) % n;
        V = (modpow(V, 2, n) - 2 * modpow(Q, k, n)) % n;
        k *= 2;
        if (V == 0)
            return true;
    }

    return false;
}

int jacobi(int a, int n) {
    assert(n > 0);

    if (n == 1)
        return 1;

    a %= n; // Rule 2

    if ((a == 0) or (a == 1))
        return a;

    // Rule 3
    if (std::__gcd(a, n) != 1)
        return 0;

    // Rule 8
    if (a == 2)
        switch (n % 8) {
            case 1: case 7:
                return 1;
            case 3: case 5:
                return -1;
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

int findD(int n) {
    int D = 5;
    while (jacobi(D, n) != -1) {
        D += (D < 0) ? -2 : 2;
        D *= -1;
    }
    return D;
}

bool millerRabin(int n, int b) {
    int d = n - 1;
    int r = 0;

    while (d % 2 == 0) {
        d /= 2;
        r += 1;
    }

    int x = modpow(b, d, n);

    if ((x == 1) or (x == n - 1))
         return true;

    for (int i = 0; i < r - 1; i++) {
        x = modpow(x, 2, n);
        if (x == 1)
            return false;
        if (x == n - 1)
            return true;
    }

    return false;
}

int _pollardRho(int n, int x, int c) {
    int y = x;
    int factor = 1;
    int cycle_size = 2;

    while (factor == 1) {
        for (int count = 1; count <= cycle_size && factor <= 1; count++) {
            x = modpow(x, 2, n) + c % n;
            factor = std::__gcd(abs(x - y), n);
        }

        if (factor == n)
            return _pollardRho(n, (rand() % (n - 2)) + 2, (rand() % (n - 1)) + 1);

        cycle_size *= 2;
        y = x;
    }

    return factor;
}

int pollardRho(int n) {
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
        int factor = pollardRho(n / m);
        ret.push_back(factor);
        m *= factor;
    }

    return ret;
}
