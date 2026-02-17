#include <gtest/gtest.h>
#include "myClass.h"

// Test case for the constructor and getValue
TEST(MyClassTest, ConstructorAndGetValue) {
    MyClass obj(10);
    EXPECT_EQ(obj.getValue(), 10);
}

// Test case for setValue
TEST(MyClassTest, SetValue) {
    MyClass obj(10);
    obj.setValue(42);
    EXPECT_EQ(obj.getValue(), 42);
}

// Test case for greet
TEST(MyClassTest, Greet) {
    MyClass obj(0);
    EXPECT_EQ(obj.greet("Alice"), "Hello, Alice!");
}

