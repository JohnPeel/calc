#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <limits>
#include "Float.h"
#include "Integer.h"
#include "Addition.h"
#include "Multiplication.h"
#include "Division.h"

double Float::getValue() {
    return value;
}

Float::Float(double value) {
    this->value = value;
}

Expression* Float::simplify() {
    if (value < 0)
        return (new Multiplication(negOne, (new Float(-value))->simplify()))->simplify();

    int wholeNumber = (int)std::floor(value);
    double fractional = value - wholeNumber;

    if (wholeNumber > 0) {
        if (fractional != 0)
            return new Addition(new Integer(wholeNumber), (new Float(fractional))->simplify());
        else
            return new Integer(wholeNumber);
    }

    unsigned long long num = 0, den = 0;
    if ((richards(fractional, num, den, 1e-6) > 0) && (num < std::numeric_limits<int>::max()) && (den < std::numeric_limits<int>::max()))
        return (new Division(new Integer((int)num), new Integer((int)den)))->simplify();

    return new Float(fractional);
}

int getDecimalCount(double value) {
    int count = 0;
    while (std::floor(value * pow(10, 6)) / pow(10, 6) != std::floor(value)) {
        value *= 10;
        count++;
    }

    return std::min(std::max(count, 1), 6);
}

std::string Float::getString() {
    std::ostringstream ss;
    ss.precision(getDecimalCount(value));
    ss << value;
    return ss.str();
}
