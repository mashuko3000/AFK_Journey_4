
#include "helpers/NumberTheoryService.hpp"

int main()
{
    bigint a = 114;
    bigint b = 39;

    NumberTheoryService num;
    bigint c = num.gcd(a, b);

    std::cout << c << std::endl;
    return 0;
}