#include <cmath>
#include <algorithm>
#include <sstream>
#include "Float.h"
#include "Division.h"
#include "Integer.h"
#include "Addition.h"
#include "Utility.h"

double Float::getValue() {
    return value;
}

Float::Float(double value) {
    this->value = value;
}

Expression* Float::simplify() {
    int wholeNumber = (int)std::floor(value);
    double fractional = value - wholeNumber;

    long precision = 1000000000;

    int common = (int)gcd<long>((long)std::round(fractional * precision), precision);

    if (common == 1)
        return this;

    int den = precision / common;
    int num = (int)std::round(fractional * precision) / common;

    return (new Addition(new Integer(wholeNumber), new Division(new Integer(num), new Integer(den))))->simplify();
}

int getDecimalCount(double value) {
    int count = 0;
    while (value != std::floor(value)) {
        value *= 10;
        count++;
    }

    return std::min(std::max(count, 1), 6);
}

std::string Float::getString() {
    char buffer[50];
    sprintf(buffer, "%#.*f",  getDecimalCount(value), value);
    return std::string(buffer);
}
