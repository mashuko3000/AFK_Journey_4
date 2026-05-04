#include "../inc/bigfloat.hpp"

using namespace std;
// Расчёт Пи по формуле Рамануджана
bigfloat bigfloat::PI(const bigfloat& eps) {
    if(eps <= bigfloat(0)){
        throw std::invalid_argument("Epsilon should be more than 0, dolbayob");
    }
    big_int one(1);
    bigfloat sum(0);


    bigfloat const_1103(1103); // the constant from the formula
    bigfloat const_26390(26390); // the constant from the formula
    bigfloat const_396_4 = bigfloat(396).pow(4); // (4 * 99)^4
    bigfloat const_2 = bigfloat(2); // the constant from the formula
    bigfloat const_9801(9801); // the constant from the formula

    bigfloat sqrt_2 = bigfloat(big_int(2), big_int(1)).sqrt(eps) * const_2; // 2√2

    unsigned long k = 0;

    while (true) {
        big_int factorial_4k = bigfloat::factorial(4 * k); // (4k)!
        big_int factorial_k = bigfloat::factorial(k);     // k!
        bigfloat pow_396_4k = bigfloat(396).pow(4 * k); // 396^(4k)

        bigfloat numerator = bigfloat(factorial_4k) * (const_1103 + const_26390 * bigfloat(k));
        bigfloat denominator = bigfloat(factorial_k);
        denominator = denominator.pow(4); // (k!)^4
        denominator *= pow_396_4k; // (k!)^4 * (4 * 99)^4k

        bigfloat current = numerator / denominator;

        sum += current; // new member of series

        // check if the absolute value of 'current' is below the epsilon threshold
        // this means we've reached an acceptable solution/approximation
        if (current.abs() < eps) {
            break; // exit the loop
        }

        //safety check to prevent infinite loops
        ++k;
        /*
        if (k > 100000) {
            throw std::runtime_error("PI_Ramanujan: max iterations exceeded");
        }*/
    }

    //end of the formula
    bigfloat sum_ = sqrt_2 * sum;
    bigfloat pi = const_9801 / sum_;
    pi.simplify();

    return pi;
}

bigfloat sin(const bigfloat &base, const bigfloat &eps){
    // Validate precision
    if (eps <= bigfloat(0)) {
        throw std::invalid_argument("Epsilon must be positive for sin computation");
    }

    // Compute π, 2π, and π/2 with specified precision
    bigfloat pi = bigfloat::PI(eps);
    bigfloat two_pi = pi * bigfloat(2);
    bigfloat half_pi = pi / bigfloat(2);

    // Normalize input angle to [-2π, 2π]
    bigfloat x = base;
    if (x.abs() > two_pi) {
        x = x % two_pi; // Reduce to [-2π, 2π]
    }

    // Handle negative angles: sin(-x) = -sin(x)
    bool negate = false;
    if (x < bigfloat(0)) {
        x = -x;
        negate = true;
    }

    // Reduce angle to [0, π]
    if (x > pi) {
        // sin(x) = -sin(2π - x) for x in (π, 2π]
        x = two_pi - x;
        negate = !negate; // Flip sign
    }

    // Reduce angle to [0, π/2]
    if (x > half_pi) {
        // sin(x) = sin(π - x) for x in (π/2, π]
        x = pi - x;
    }

    // Compute sin(x) using Taylor series
    bigfloat result = x; // First term: x
    bigfloat term = x; // Current term
    bigfloat x_squared = x.pow(2); // x^2 for efficiency
    unsigned long n = 1; // Term counter
    const int max_iter = 1000000; // Prevent infinite loops
    int iter = 0;

    // Series: sin(x) = x - x^3/3! + x^5/5! - ...
    // Each term: a_n = a_{n-1} * (-x^2) / (2n * (2n+1))
    while (term.abs() > eps) {
        term = term * (-x_squared) / bigfloat(2 * n * (2 * n + 1));
        result += term;
        n++;
        iter++;
        if (iter > max_iter) {
            throw std::runtime_error("sin: series did not converge within max iterations");
        }
        // Simplify to maintain numerical stability
        result.simplify();
        term.simplify();
    }

    // Apply sign based on original angle
    return negate ? -result : result;
}

bigfloat cos(const bigfloat &x, const bigfloat &eps) {
    bigfloat pi = bigfloat::PI(eps); // π
    bigfloat two_pi = pi * bigfloat(2); // 2π
    bigfloat x_reduced = x; // number for decrease
    bool negate = false;

    if (x_reduced.abs() > two_pi) {
        x_reduced = x_reduced % (two_pi);
    }

    // negative input, like (cos(-x) = cos(x))
    if (x_reduced < bigfloat(0)) {
        x_reduced = -x_reduced;
    }

    // reducing to [0, 2PI]
    if (x_reduced > pi) {
        x_reduced = two_pi - x_reduced; // this is like cos(x) = cos(2PI - x)
    }
    if (x_reduced > pi / bigfloat(2)) {
        x_reduced = pi - x_reduced; // this is like cos(x) = -cos(PI - x)
        negate = !negate;
    }

    // Taylor series
    // cos(x) = Σ (-1)ⁿ x²ⁿ/(2n)!
    bigfloat term(1);
    bigfloat result(1);
    bigfloat x_squared = x_reduced.pow(2);
    unsigned long n = 1;
    int sign = -1;

    while (term.abs() > eps) {
        // each term calculate like this a(n) = a(n - 1) * x² / ((2n - 1) * 2n)
        bigfloat denom = bigfloat((2 * n - 1) * (2 * n));
        term = term * x_squared / denom;
        result += (sign > 0 ? term : -term);
        sign = -sign;
        ++n;
    }

    result.simplify();
    return negate ? -result : result;
}

bigfloat binomialCoefficient(unsigned long n, unsigned long k) {
    if (k > n) {
        throw std::invalid_argument("k cannot be greater than n in binomial coefficient");
    }

    bigfloat res(1);
    if (k > n - k) k = n - k;
    for (unsigned long i = 0; i < k; ++i) {
        res *= bigfloat(n - i);
        res /= bigfloat(i + 1);
    }
    res.simplify();
    return res;
}

bigfloat bernoulli(unsigned long n) {
    static bigfloat* cache = nullptr;
    static unsigned long cache_size = 0;

    if (cache == nullptr) {
        cache_size = 64;
        try {
            cache = new bigfloat[cache_size];
            for (unsigned long i = 0; i < cache_size; ++i) {
                cache[i] = bigfloat(0);
            }
            // Set base cases
            cache[0] = bigfloat(1);
            cache[1] = bigfloat("-1/2");
        } catch (const std::bad_alloc& e) {
            throw std::bad_alloc();
        }
    }

    if (n >= cache_size) {
        unsigned long new_size = n * 2;
        bigfloat* new_cache = nullptr;
        try {
            new_cache = new bigfloat[new_size];
            for (unsigned long i = 0; i < new_size; ++i) {
                new_cache[i] = (i < cache_size) ? cache[i] : bigfloat(0);
            }
            delete[] cache;
            cache = new_cache;
            cache_size = new_size;
        } catch (const std::bad_alloc& e) {
            delete[] new_cache;
            throw std::bad_alloc();
        }
    }

    if (cache[n] != bigfloat(0) || n == 0 || n == 1) {
        return cache[n];
    }

    if (n % 2 != 0) {
        cache[n] = bigfloat(0);
        return cache[n];
    }

    // Compute B_n using recursive sum
    bigfloat sum(0);
    for (unsigned long k = 0; k < n; ++k) {
        // Compute binomial coefficient C(n+1, k)
        bigfloat coef = binomialCoefficient(n + 1, k);
        sum += coef * bernoulli(k);
    }

    cache[n] = -sum / bigfloat(n + 1);
    cache[n].simplify();
    return cache[n];
}


// series only for |x| < pi\2
bigfloat tan(const bigfloat &base, const bigfloat &eps) {
    bigfloat pi = bigfloat::PI(eps);
    bigfloat two(2);
    bigfloat two_pi = pi * two;
    bigfloat half_pi = pi / two;
    bigfloat x = base;
    bool negate = false;

    // (tan(x) = tan(x mod π)) reduce to [-π, π]
    if (x.abs() > pi) {
        x = x % (pi);
    }

    if (x > half_pi) { // (π/2, π]: tan(x) = -tan(π - x)
        x = pi - x;
        negate = !negate;
        x.simplify();
    } else if (x <= -half_pi) { // [-π, -π/2): tan(x) = tan(π + x)
        x = pi + x;
        negate = !negate;
        x.simplify();
    }
    if (x < bigfloat(0)) { // (tan(-x) = -tan(x))
        x = -x;
        negate = !negate;
        x.simplify();
    }

    if (x.abs() > half_pi - eps) {
        throw std::invalid_argument("Tangent is not defined in π/2 + kπ");
    }


    // tan(x) = Σ [B₂ₙ * (-4)ⁿ * (1-4ⁿ)/(2n)!] * x²ⁿ⁻¹
    bigfloat result(0);
    bigfloat term;
    bigfloat x_squared = x.pow(2);
    bigfloat x_power = x;
    bigfloat pow_neg4_n = bigfloat(-4);
    bigfloat pow_4_n = bigfloat(4);
    unsigned long n = 1;

    while (true) {
        bigfloat Bn = bernoulli(2 * n);
        // B₂ₙ * (-4)ⁿ * (1 - 4ⁿ)
        bigfloat coef = Bn * pow_neg4_n * (bigfloat(1) - pow_4_n);
        // (2n)!
        bigfloat denom = factorial(2 * n);
        // (coef/(2n)!) * x^(2n-1)
        term = (coef / denom) * x_power;

        if (term.abs() < eps) {
            break;
        }

        result += term;
        ++n;
        x_power *= x_squared;
        pow_neg4_n *= bigfloat(-4);
        pow_4_n *= bigfloat(4);

        if (n > 300) {
            throw std::runtime_error("tan: max iterations exceeded");
        }
    }
    result.simplify();
    return negate ? -result : result;
}

bigfloat cot(const bigfloat &base, const bigfloat &eps){
    bigfloat pi = bigfloat::PI(eps);
    bigfloat x = base;

    if (x < bigfloat(0)) {
        x = -x;  // cot(-x) = -cot(x)
    }
    if (x > pi / bigfloat(2)) {
        x = pi - x; // reduce to [0, PI/2]
    }

    if (x.abs() < eps) {
        throw std::invalid_argument("Cotangent is not defined near k*PI");
    }
    // cot(π/2) = 0, but tan is not defined
    if ((x - pi / bigfloat(2)).abs() < eps) {
        return bigfloat(0);
    }

    bigfloat tan_val = tan(x, eps);

    if (tan_val.abs() < eps) {
        throw std::invalid_argument("Cotangent is not defined near PI/2 + k*PI");
    }
    return tan_val.reciprocal();
}
bigfloat sec(const bigfloat &base, const bigfloat &eps){
    bigfloat pi = bigfloat::PI(eps);
    bigfloat half_pi = pi / bigfloat(2);

    bigfloat x = base;
    bigfloat two_pi = pi * bigfloat(2);

    x = x % (two_pi);
    if (x < bigfloat(0)) x += two_pi; // convert to positive equivalent angle, like cos
    // x is close to π/2 or to -π/2
    bigfloat distance_to_pole = (x % half_pi).abs();
    if (distance_to_pole < eps ||
        (half_pi - distance_to_pole).abs() < eps) {
        throw std::invalid_argument("Secant is undefined at π/2 + kπ");
    }
    bigfloat cos_val = cos(x, eps);
    return cos_val.reciprocal();
}

bigfloat csc(const bigfloat &base, const bigfloat &eps){
    bigfloat sin_val = sin(base, eps);
    if (sin_val.abs() < eps) {
        throw std::invalid_argument("Cosecant is not defined in kπ");
    }
    return sin_val.reciprocal();
}
bigfloat arcsin(const bigfloat &base, const bigfloat &eps) {
    if (base.abs() > bigfloat(1)) {
        throw std::invalid_argument("arcsin is defined only for |x| <= 1");
    }

    bigfloat pi = bigfloat::PI(eps);
    bigfloat half_pi = pi / bigfloat(2);

    //base cases
    if (base == bigfloat(1)) return half_pi;
    if (base == bigfloat(-1)) return -half_pi;
    if (base == bigfloat(0)) return bigfloat(0);

    bigfloat result = base;
    bigfloat term = base;
    bigfloat x_squared = base.pow(2);
    unsigned long n = 1;

    // arcsin(x) = x + Σ [((2n-1)!! / (2n)!!) * x^(2n+1) / (2n+1)]
    while (true) {
        // a(n) = a(n-1) * (2n-1) * x^2 / (2 * n)
        term = term * x_squared * bigfloat(2 * n - 1) / bigfloat(2 * n);
        // a(n) / (2n+1)
        bigfloat next_term = term / bigfloat(2 * n + 1);
        if (next_term.abs() < eps) {
            break;
        }
        result += next_term;
        ++n;
    }
    result.simplify();
    return result;
}

bigfloat arccos(const bigfloat &base, const bigfloat &eps) {
    if (base.abs() > bigfloat(1)) {
        throw std::invalid_argument("arccos is defined only for |x| <= 1");
    }

    bigfloat pi = bigfloat::PI(eps);
    bigfloat half_pi = pi / bigfloat(2);

    // basic cases
    if (base == bigfloat(1)) return bigfloat(0);
    if (base == bigfloat(-1)) return pi;
    if (base == bigfloat(0)) return half_pi;

    bigfloat x = base;

    bigfloat arcsin_result = x;
    bigfloat term = x;
    bigfloat x_squared = x * x;
    big_int n = 1;
    // arcsin(x) = x + Σ [((2n-1)!! / (2n)!!) * x^(2n+1) / (2n+1)]
    while (true) {
        // x^(2n+1) * (2n-1)!! / (2n)!!
        term = term * x_squared * bigfloat(2 * n - 1) / bigfloat(2 * n);
        // a(n) / (2n+1)
        bigfloat next_term = term / bigfloat(2 * n + 1);

        if (next_term.abs() < eps) {
            break;
        }

        arcsin_result += next_term;
        ++n;

        if (n > 10000000) {
            throw std::runtime_error("arccos: max iterations exceeded (arcsin part)");
        }
    }

    // arccos = π/2 - arcsin(x)
    bigfloat result = half_pi - arcsin_result;
    result.simplify();

    return result;
}

bigfloat arctg(const bigfloat &x, const bigfloat &eps) {
    if (eps <= bigfloat(0)) {
        throw std::invalid_argument("Epsilon must be positive for arctg computation");
    }

    // basic cases
    if (x == bigfloat(0)) {
        return bigfloat(0);
    }
    if (x == bigfloat(1)) {
        return bigfloat::PI(eps) / bigfloat(4);
    }
    if (x == bigfloat(-1)) {
        return -bigfloat::PI(eps) / bigfloat(4);
    }

    bigfloat result = x;
    bigfloat term = x;
    bigfloat x_squared = x * x;
    big_int n = 1;
    bigfloat sign = bigfloat(-1);
    // for |x| > 1, use arctg(x) = ±π/2 - arctg(1/x)
    if (x.abs() > bigfloat(1)) {
        bigfloat half_pi = bigfloat::PI(eps) / bigfloat(2);
        bigfloat recip = x.reciprocal();
        result = (x > bigfloat(0)) ? half_pi - arctg(recip, eps)
                                    : -half_pi - arctg(recip, eps);
        // for 0.5 < |x| <= 1 use arctg(x) = π/4 + arctg((x-1)/(x+1))
    } else if (x.abs() > bigfloat("1/2")) {
        bigfloat arg = (x - bigfloat(1)) / (x + bigfloat(1));
        result = bigfloat::PI(eps) / bigfloat(4) + arctg(arg, eps);
        if (x < bigfloat(0)) {
            result = -result;
        }
    } else {
        //arctg(x) = x - x^3/3 + x^5/5 - ...
        while (term.abs() > eps) {
            term *= x_squared;
            bigfloat next_term = sign * term / bigfloat(2 * n + 1);

            if (next_term.abs() < eps) {
                break;
            }

            result += next_term;
            sign = -sign;
            ++n;

            if (n > 100000) {
                throw std::runtime_error("arctg: maximum iterations exceeded");
            }
        }
    }

    result.simplify();
    return result;
}

bigfloat arcctg(const bigfloat &base, const bigfloat &eps){
    if (eps <= bigfloat(0)) {
        throw std::invalid_argument("Epsilon must be positive for arcctg computation");
    }
    bigfloat pi = bigfloat::PI(eps);
    bigfloat half_pi = pi / bigfloat(pi);

    if (base == bigfloat(0)) {
        return half_pi;
    }

    bigfloat x = base;

    if (x == bigfloat(0)) {
        return pi / bigfloat(2);
    }

    bigfloat inv_x = x.reciprocal();
    bigfloat arctan_result = arctg(inv_x, eps);

    bigfloat result;
    // For x > 0: arccot(x) = arctan(1/x)
    // For x < 0: arccot(x) = π + arctan(1/x)
    result = (base > bigfloat(0)) ? arctan_result : pi + arctan_result;

    result.simplify();
    return result;
}

bigfloat arcsec(const bigfloat &base, const bigfloat &eps){
    if (eps <= bigfloat(0)) {
        throw std::invalid_argument("Epsilon must be positive for arcsec computation");
    }

    if (base.abs() < 1) {
        throw std::invalid_argument("arcsec is defined only for |x| >= 1");
    }

    if (base == bigfloat(1)) {
        return bigfloat(0);
    }

    if (base == bigfloat(-1)) {
        return bigfloat::PI(eps);
    }

    return arccos(base.reciprocal(), eps);
}
bigfloat arccosec(const bigfloat &base, const bigfloat &eps){
    if (eps <= bigfloat(0)) {
        throw std::invalid_argument("Epsilon must be positive for arccosec computation");
    }

    if (base.abs() < bigfloat(1)) {
        throw std::invalid_argument("Arccosecant is defined only for |x| >= 1");
    }

    bigfloat half_pi = bigfloat::PI(eps) / bigfloat(2);
    if (base == bigfloat(1)) {
        return half_pi;
    }
    if (base == bigfloat(-1)) {
        return -half_pi;
    }
    return arcsin(base.reciprocal(), eps);
}

