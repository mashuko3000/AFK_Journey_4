
#include "helpers/NumberTheoryService.hpp"
#include "primality_tests/FermatPrimalityTest.hpp"

int main()
{
    bigint a = 114;
    bigint b = 39;

    NumberTheoryService num;
    FermatPrimalityTest test;

    std::cout << test.isPrime(104728, 0.99);

    bigint c = num.gcd(a, b);

    std::cout << c << std::endl;
    return 0;
}