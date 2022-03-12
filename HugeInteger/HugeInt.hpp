#pragma once
#include <iostream>
#include <vector>

class HugeInt {
private:
    std::string digits;
public:
    HugeInt(std::string value);
    HugeInt(int value);
    HugeInt operator+(const HugeInt& arg);
    HugeInt operator*(const HugeInt& arg);
    friend std::ostream& operator<<(std::ostream& out, const HugeInt& obj);
};