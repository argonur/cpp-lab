#include "myClass.h"

MyClass::MyClass(int value) : m_value(value) {}

int MyClass::getValue() const {
    return m_value;
}

void MyClass::setValue(int newValue) {
    m_value = newValue;
}

std::string MyClass::greet(const std::string& name) const {
    return "Hello, " + name + "!";
}
