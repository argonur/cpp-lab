#include "millisecond.h"

Millisecond::Millisecond(int v) : value(v) {}

Millisecond Millisecond::operator*(const Millisecond& other) const {
    return Millisecond(value * other.value);
}

int Millisecond::get() const { return value; }