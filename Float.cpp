#include "Float.h"

double Float::getValue() {
    return value;
}

Float::Float(float value) {
    this->value = value;
}

std::string Float::getString() {
    return std::to_string(value);
}

double Double::getValue() {
    return value;
}

Double::Double(double value) {
    this->value = value;
}

std::string Double::getString() {
    return std::to_string(value);
}