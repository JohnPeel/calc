#include <cmath>
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

    // TODO: lucas test!

    return true;
}

int jacobi(int a, int n) {
    // Simple Answers
    if (n == 1) return 1;
    if ((a == 0) or (a == 1)) return a;

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

    // Rule 4 and Rule 7
    if (a < 0)
        return (int)std::pow(-1, (n - 1) >> 1) * jacobi(-a, n);

    // Rule 4
    if (not a & 1)
        return jacobi(2, n) * jacobi(a >> 1, n);

    // Rule 2 (with a check that we're not calculating the same a)
    if (a % n != a)
        return jacobi(a % n, n);

    // Rule 6
    if ((a % 4 == 3) and (n % 4 == 3))
        return -jacobi(n, a);
    else
        return jacobi(n, a);
}

bool millerRabin(int n, int b) {
    int d = n - 1;
    int r = 0;

    while (not d & 1) {
        d = d >> 1;
        r += 1;
    }

    int x = modular_pow(b, d, n);

    if ((x == 1) or (x == n - 1))
         return true;

    for (int i = 0; i < r - 1; i++) {
        x = modular_pow(x, 2, n);
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
            x = modular_pow(x, 2, n) + c % n;
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
