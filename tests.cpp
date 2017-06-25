#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Prime.h"
TEST_CASE("Prime", "[prime]") {
    SECTION("Miller-Rabin primality test") {
        SECTION("Miller-Rabin 5->99") {
            INFO("Checking Miller-Rabin on first 100 numbers.");
            for (int i = 5; i < 100; i += 6) {
                INFO("Checking " << i);
                CHECK(millerRabin(i) == isProbablePrime(i));
                CHECK(millerRabin(i, 3) == isProbablePrime(i));

                INFO("Checking " << i + 2);
                CHECK(millerRabin(i + 2) == isProbablePrime(i + 2));
                CHECK(millerRabin(i + 2, 3) == isProbablePrime(i + 2));
            }
        }

        SECTION("Strong base-2 pseudoprimes") {
            INFO("Checking Strong base-2 pseudoprimes");
            for (int i : {2047, 3277, 4033, 4681, 8321}) {
                INFO("Checking " << i);
                CHECK(millerRabin(i));
                CHECK_FALSE(isProbablePrime(i));
            }
        }

        SECTION("Strong base-3 pseudoprimes") {
            INFO("Checking Strong base-3 pseudoprimes");
            for (int i : {121, 703, 1891, 3281, 8401}) {
                INFO("Checking " << i);
                CHECK(millerRabin(i, 3));
                CHECK_FALSE(isProbablePrime(i));
            }
        }
    }

    SECTION("Strong Lucas pseudoprimes") {
        /*SECTION("Lucas 5->99") {
            INFO("Checking Lucas on first 100 numbers.");
            for (int i = 5; i < 100; i += 6)
                if (std::sqrt(i) != std::round(std::sqrt(i))) {
                    INFO("Checking " << i);
                    CHECK(lucasPP(i) == isProbablePrime(i));

                    INFO("Checking " << i + 2);
                    CHECK(lucasPP(i + 2) == isProbablePrime(i + 2));
                }
        }*/

        INFO("Checking Strong lucas pseudoprimes");
        for (int i : {5459, 5777, 10877, 16109, 18971}) {
            INFO("Checking " << i);
            CHECK(lucasPP(i));
            CHECK_FALSE(isProbablePrime(i));
        }
    }
}

#include "Utility.h"
TEST_CASE("Utility", "[utility]") {
    // Stub
}

#include "Expression.h"
#include "Integer.h"
#include "Float.h"
#include "Variable.h"
#include "Method.h"
#include "Addition.h"
#include "Subtraction.h"
#include "Multiplication.h"
#include "Division.h"
#include "Exponentiation.h"
#include "Logarithm.h"

#include "Parser.h"
#include "ShuntingYard.h"
TEST_CASE("ShuntingYard", "[shuntingyard]") {
    // TODO: Write this.
}

#define test(a, b) CHECK(simplify(a) == b);

TEST_CASE("CAS") {
    SECTION("Factoring") {
        SECTION("GCF") {
            test("2(x ^ 2) + 3(x ^ 2)", "5(x ^ 2)");
            test("3pi + 5pi", "8pi");

            test("(x ^ (e + 1)) / (x ^ 1)", "x ^ e");
        }

        SECTION("Sum of Cubes") {
            test("(x ^ 3) + 8", "((x ^ 2) - 2x + 4)(x + 2)");
        }

        SECTION("Difference of Cubes") {
            test("(x ^ 3) - 8", "((x ^ 2) + 2x + 4)(x - 2)");
        }

        SECTION("Difference of Squares") {
            test("((x ^ 2) - 4) / (x - 2)", "x + 2");
            test("(x ^ 2) - 4", "(x + 2)(x - 2)");
            test("(x ^ 4) - 16", "(((x ^ 2) + 4)(x + 2))(x - 2)");
        }

        SECTION("Quadratic") {
            test("-(x ^ 2) + 4x - 4", "(x - 2) ^ 2");
            test("(x ^ 2) + 4x + 4", "(x + 2) ^ 2");
            test("(x ^ 2) - 4x + 4", "(x - 2) ^ 2");
            test("(x ^ 2) + 5x + 6", "(x + 3)(x + 2)");
            test("(x ^ 2) - 5x + 6", "(x - 2)(x - 3)");
            test("(x ^ 2) + x - 6", "(x + 3)(x - 2)");
            test("(x ^ 2) - x - 6", "(x + 2)(x - 3)");
        }
    }

    SECTION("Complex Numbers") {
        test("2 rt (-4)", "2i");
        test("(2 rt (-4))(2 rt (-4))", "-4");
        test("i * (3i + 2)", "2i - 3");
    }

    SECTION("Distribution") {
        test("2(3 + 2x)", "6 + 4x");
    }


    SECTION("Project Overview") {
        test("(e - 1) / ((e ^ 2) - 1)", "1 / (e + 1)");
        test("2 rt ((pi ^ 2) + 2pi + 1)", "pi + 1");
        test("2--3", "5");
        test("1.25", "1 + (1 / 4)");
        test("0.14285714285", "0.142857");
        test("0.5", "1 / 2");
        test("1.0", "1");
    }
}