//
// Created by mvideo on 27/02/26.
//

#ifndef NTA_2_CONVERT_HPP
#define NTA_2_CONVERT_HPP

#include<cstdint>
#include<string>
#include <vector>
#include <stdexcept>
#include<iostream>
#include <algorithm>
#include <sstream>

#include"../inc/convert.hpp"

std::string to_polynomial(uint64_t elem, int n);
uint64_t from_polynomial(const std::string& str, int n);
uint64_t polynomial_multiply_no_reduce(uint64_t a, uint64_t b);
int polynomial_degree(uint64_t p);
uint64_t polynomial_reduce(uint64_t poly, uint64_t modulus, int n);
uint64_t multiply(uint64_t a, uint64_t b, uint64_t irreducible, int n);
struct gcd
{
    uint64_t gcd;
    uint64_t s;
    uint64_t t;
};
gcd extended_gcd(uint64_t a, uint64_t b, uint64_t mod_poly, int n);



#endif //NTA_2_CONVERT_HPP
