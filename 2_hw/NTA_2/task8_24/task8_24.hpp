//
// Created by mvideo on 27/02/26.
//

#ifndef NTA_2_TASK8_24_HPP
#define NTA_2_TASK8_24_HPP
#include <iostream>
#include <vector>
#include <iomanip>

#include <set>
#include<cstdlib>
#include<chrono>

using namespace std;
using ll = long long;

const ll MOD = 998244353;
const ll INV2 = 499122177LL;

vector<ll> fast_convolution_rec(
        const vector<ll> f,
        const vector<ll> g,
        int n //degree of 2
);
ll power(ll base, ll exp);
vector<ll> multiply_cyclic(const vector<ll>& a, const vector<ll>& b);
void task8_24();
void task30_1_7(int n_size);
#endif //NTA_2_TASK8_24_HPP
