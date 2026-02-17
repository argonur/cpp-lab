#include <iostream>
#include "myClass.h"

int main() {
    MyClass obj(10);
    std::cout << "Initial Value: " << obj.getValue() << std::endl;

    obj.setValue(42);
    std::cout << "Updated Value: " << obj.getValue() << std::endl;

    std::cout << obj.greet("World") << std::endl;

    return 0;
}
