#ifndef MYCLASS_H
#define MYCLASS_H

#include <string>

class MyClass {
public:
    MyClass(int value);
    int getValue() const;
    void setValue(int newValue);
    std::string greet(const std::string& name) const;

private:
    int m_value;
};

#endif // MYCLASS_H