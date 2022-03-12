#include "HugeInt.hpp"

int main() {
    HugeInt a(14143);
    HugeInt b("12121212");
    std::cout << a*b;
    return 0;
}