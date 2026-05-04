#include "../inc/bigfloat.hpp"

bigfloat bigfloat::E(const bigfloat& eps, const bigfloat& x)
{
    if (eps <= bigfloat(0))
    {
        throw std::invalid_argument("Epsilon must be positive");
    }

    bigfloat result(1);
    bigfloat term(1);
    big_int n = 1;

    while(true)
    {
        //std::cout << result << std::endl;
        term = term * x / bigfloat(n);
        if(term.abs() < eps)
        {
            break;
        }
        result +=term;
        n++;
        //if(n % 10 == 0) result.simplify();
    }

    return result;
}