#pragma once

class Millisecond {

private:
    int value;

public:
    explicit Millisecond(int v);

    Millisecond operator*(const Millisecond& other) const;

    int get() const;
};