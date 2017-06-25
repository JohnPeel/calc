#include <iostream>
#include <cassert>
#include <iomanip>
#include "ShuntingYard.h"
#include "Integer.h"
#include "Utility.h"

using namespace std;

bool useColor = false;

string green(string str) { return ((useColor) ? "\x1B[32;1m" : "") + str + ((useColor) ? "\x1B[0m" : ""); }
string red(string str) { return ((useColor) ? "\x1B[31;1m" : "") + str + ((useColor) ? "\x1B[0m" : ""); }

void test(std::string expr, std::string expectedStr, std::string simplifiedStr) {
    Expression* testExpr = ShuntingYard(expr).process();
    std::string testExprStr = testExpr->getString(), testExprSStr = testExpr->simplify()->getString();
    bool passed = (testExprStr == expectedStr) and (testExprSStr == simplifiedStr);


    std::string str = (passed ? green("PASSED") : red("FAILED")) + ": \"" + expr + "\"";
    while (str.length() < 35) str += " ";
    str += " -> ";
    while (str.length() < 40) str += " ";

    str += "\"" + testExprStr + "\"";
    if (testExprStr != expectedStr)
        str += " <-> \"" + expectedStr + "\"";
    while (str.length() < 75) str += " ";
    str += " -> ";
    while (str.length() < 80) str += " ";

    str += "\"" + testExprSStr + "\"";
    if (testExprSStr != simplifiedStr)
        str += " <-> \"" + simplifiedStr + "\"";

    cout << str << endl;
}

void test(std::string expr, std::string simplifiedStr) {
    test(expr, expr, simplifiedStr);
}

void test(std::string expr) {
    test(expr, expr);
}

void testing() {
    // Sum of cubes
    test("(x ^ 3) + 8", "((x ^ 2) - 2x + 4)(x + 2)");
    // Diff of cubes
    test("(x ^ 3) - 8", "((x ^ 2) + 2x + 4)(x - 2)");

    // Negative even root
    test("2 rt (-4)", "2i");

    // i * i
    test("(2 rt (-4))(2 rt (-4))", "-4");
    test("i * (3i + 2)", "i(3i + 2)", "2i - 3");

    // Diff of Squares
    test("((x ^ 2) - 4) / (x - 2)", "x + 2");
    test("(x ^ 2) - 4", "(x + 2)(x - 2)");
    test("(x ^ 4) - 16", "(((x ^ 2) + 4)(x + 2))(x - 2)");

    // Quadratic
    test("-(x ^ 2) + 4x - 4", "(x - 2) ^ 2");
    test("(x ^ 2) + 4x + 4", "(x + 2) ^ 2");
    test("(x ^ 2) - 4x + 4", "(x - 2) ^ 2");
    test("(x ^ 2) + 5x + 6", "(x + 3)(x + 2)");
    test("(x ^ 2) - 5x + 6", "(x - 2)(x - 3)");
    test("(x ^ 2) + x - 6", "(x + 3)(x - 2)");
    test("(x ^ 2) - x - 6", "(x + 2)(x - 3)");

    // Distribution
    test("2(3 + 2x)", "6 + 4x");

    // Factoring by GCF
    test("2(x ^ 2) + 3(x ^ 2)", "5(x ^ 2)");
    test("3pi + 5pi", "8pi");

    // Tests from Project Overview
    test("(e - 1) / ((e ^ 2) - 1)", "1 / (e + 1)");
    test("2 rt ((pi ^ 2) + 2pi + 1)", "pi + 1");
    test("2--3", "2 - (-3)", "5");
    test("1.25", "1 + (1 / 4)");
    test("0.14285714285", "0.142857", "0.142857");
    test("0.5", "1 / 2");
    test("1.0", "1");

    // Exponential Factors (GCF)
    test("(x ^ (e + 1)) / (x ^ 1)", "x ^ e");
}

class InputParser{
protected:
    vector<string> tokens;
public:
    InputParser(int argc, char* argv[]) {
        for (int i = 1; i < argc; i++)
            tokens.push_back(string(argv[i]));
    }

    bool hasParameter(string option){
        return find(tokens.begin(), tokens.end(), option) != tokens.end();
    }
};

int main(int argc, char* argv[]) {
    InputParser input(argc, argv);
    if (input.hasParameter("-c"))
        useColor = true;

    testing();

    //for (int i = 0; i < 10; i++)
    //    test("(x ^ 2) - 4", "(x + 2)(x - 2)");

    return 0;
};