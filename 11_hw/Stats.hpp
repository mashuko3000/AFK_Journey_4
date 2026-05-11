
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/fwd.hpp>

using namespace boost::multiprecision;
using bigint = cpp_int;

using bigfloat = cpp_rational;

struct Stats 
{
    long long ops = 0;
    void reset()
    {
        ops = 0;
    }
};   