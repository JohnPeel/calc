#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Prime.h"
TEST_CASE("Prime") {
    SECTION("Miller-Rabin primality test") {
        SECTION("Miller-Rabin 5->97") {
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

    SECTION("Lucas probable prime test") {
        SECTION("Lucas 5->97") {
            INFO("Checking Lucas on first 100 numbers.");
            for (int i = 5; i < 100; i += 6) {
                if (std::sqrt(i) != std::round(std::sqrt(i))) {
                    INFO("Checking " << i);
                    CHECK(lucasPP(i) == isProbablePrime(i));
                }
                if (std::sqrt(i + 2) != std::round(std::sqrt(i + 2))) {
                    INFO("Checking " << i + 2);
                    CHECK(lucasPP(i + 2) == isProbablePrime(i + 2));
                }
            }

        }

        SECTION("Strong Lucas pseudoprimes") {
            INFO("Checking Strong lucas pseudoprimes");
            for (int i : {5459, 5777, 10877, 16109, 18971}) {
                INFO("Checking " << i);
                CHECK(lucasPP(i));
                CHECK_FALSE(isProbablePrime(i));
            }
        }
    }
}

#include "Utility.h"
#include "ShuntingYard.h"

#define test(a, b) CHECK(simplify(a) == b)

#define eval(a) strToExpr(a)->getString()
#define eval_type(a) strToExpr(a)->getTypeString()
#define eval_simplify(a) strToExpr(a)->simplify()->getString()

inline void testExpr(std::string expr, std::string typ, std::string parsedStr, std::string simplifiedStr) {
    CHECK(eval_type(expr) == typ);
    CHECK(eval(expr) == parsedStr);
    CHECK(eval_simplify(expr) == simplifiedStr);
}
inline void testExpr(std::string expr, std::string typ) { testExpr(expr, typ, expr, expr); }
inline void testExpr(std::string expr, std::string typ, std::string simplifiedStr) { testExpr(expr, typ, expr, simplifiedStr); }


TEST_CASE("Classes") {
    SECTION("Utility") {}
    SECTION("Expression") {}
    SECTION("Integer") { testExpr("1", "Integer"); }
    SECTION("Float") { testExpr("0.1", "Float", "0.1", "1 / 10"); }
    SECTION("Variable") { testExpr("x", "Variable"); }
    SECTION("Method") {}
    SECTION("Addition") { testExpr("1 + 2", "Addition", "3"); }
    SECTION("Subtraction") { testExpr("2 - 1", "Subtraction", "1"); }
    SECTION("Multiplication") { testExpr("2 * x", "Multiplication", "2x", "2x"); }
    SECTION("Division") { testExpr("2 / 2", "Division", "1"); }
    SECTION("Exponentiation") {
        testExpr("x ^ 2", "Exponentiation");
        testExpr("2 rt x", "NthRoot");
    }
    SECTION("Logarithm") {
        testExpr("2 log x", "Logarithm");
        testExpr("ln x", "Natural Log");
    }
    SECTION("Parser") {}
    SECTION("ShuntingYard") {}
}

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
            test("(x ^ 4) - 16", "((x ^ 2) + 4)(x + 2)(x - 2)");
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
        test("3 rt (-27)", "-3");
        test("4 rt (-16)", "2(4 rt (-1))");
    }

    SECTION("Distribution") {
        test("2(3 + 2x)", "6 + 4x");
    }

    SECTION("Project Overview") {
        test("(e - 1) / ((e ^ 2) - 1)", "1 / (e + 1)");
        test("2 rt ((pi ^ 2) + 2pi + 1)", "pi + 1");
        test("3 rt 8", "2");
        test("4 rt 16", "2");
        test("(6 / 2) rt (5 + 3)", "2");
        test("8 log 2", "3");
        test("(13 + 14) log (2 rt 9)", "3");
        test("2 log 8", "2 log 8");
        test("2--3", "5");
        test("1.25", "1 + (1 / 4)");
        test("0.142857142857", "1 / 7");
        test("10.2", "10 + (1 / 5)");
        test("0.5", "1 / 2");
        test("-0.5", "(-1) / 2");
        test("1.0", "1");
        test("-1.0", "-1");
    }
}