#include <cmath>
#include <algorithm>
#include <sstream>
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
    if (value < 0) {
        Expression* expr = (new Float(-value))->simplify();

        if (dynamic_cast<Float*>(expr) != NULL)
            return this;

        return (new Multiplication(negOne, expr))->simplify();
    }

    int wholeNumber = (int)std::floor(value);
    double fractional = value - wholeNumber;

    if (wholeNumber > 0) {
        if (fractional != 0)
            return new Addition(new Integer(wholeNumber), (new Float(fractional))->simplify());
        else
            return new Integer(wholeNumber);
    }

    long long precision = (long long)pow(10, 6);
    long long common = gcd((long long)std::round(fractional * precision), precision);

    if (common == 1)
        return this;

    int den = (int)(precision / common);
    int num = (int)(std::round(fractional * precision) / common);

    return new Division(new Integer(num), new Integer(den));
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
    char buffer[7]; //getDecimalCount maxes at 6, buffer for 7 to include null.
    sprintf(buffer, "%#.*f",  getDecimalCount(value), value);
    return std::string(buffer);
}
