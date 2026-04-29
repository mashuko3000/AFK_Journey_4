#include "../inc/math/NTT.hpp"

using namespace std;
using ll = long long;

vector<ll> schoolbook_multiply(const vector<ll>& a, const vector<ll>& b, ll mod)
{
    if (a.empty() || b.empty()) return {};
    int n = a.size();
    int m = b.size();
    vector<ll> res(n + m - 1, 0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
        {
            res[i + j] = (res[i + j] + (a[i] * b[j]) % mod) % mod;
        }
    }
    return res;
}

ll power(ll base, ll exp, ll m)
{
    ll res = 1;
    base %= m;
    while (exp > 0)
    {
        if (exp % 2 == 1) res = (res * base) % m;
        base = (base * base) % m;
        exp /= 2;
    }
    return (res + m) % m;
}

vector<ll> schoolbook_multiply(const vector<ll>& a, const vector<ll>& b)
{
    if (a.empty() || b.empty()) return {};
    int n = a.size();
    int m = b.size();
    vector<ll> res(n + m - 1, 0);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            res[i + j] = (res[i + j] + a[i] * b[j]) % MOD;
        }
    }
    return res;
}

vector<ll> karatsuba(
        const vector<ll>& a,
        const vector<ll>& b
        )
{
    int n = a.size();
    if (n <= 16) return schoolbook_multiply(a, b);

    int m = n >> 1;
    vector<ll> aL(a.begin(), a.begin() + m), aH(a.begin() + m, a.end());
    vector<ll> bL(b.begin(), b.begin() + m), bH(b.begin() + m, b.end());

    auto d0 = karatsuba(aL, bL); // (AL*BL)
    auto d1 = karatsuba(aH, bH); // (AH*BH)

    vector<ll> a_sum(m), b_sum(m);
    for(int i=0; i<m; ++i) {
        a_sum[i] = (aL[i] + aH[i]) % MOD; //(Al + Ah)
        b_sum[i] = (bL[i] + bH[i]) % MOD; //(BL + BH)
    }
    auto d01 = karatsuba(a_sum, b_sum); // (Al + Ah)*(BL + BH)

    vector<ll> res(2 * n - 1, 0);
    for(int i=0; i < d0.size(); ++i)
    {
        res[i] = (res[i] + d0[i]) % MOD;
        res[i + n] = (res[i + n] + d1[i]) % MOD;

        ll middle = (d01[i] - d0[i] - d1[i] + 2 * MOD) % MOD; // (Al + Ah)(BL + BH)-ALBL-AHBH
        res[i + m] = (res[i + m] + middle) % MOD;
    }
    return res;
}

ll power(ll base, ll exp)
{
    ll res = 1;
    base %= MOD;
    while (exp > 0)
    {
        if (exp & 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return res;
}

vector<ll> fast_convolution_rec(
        const vector<ll>& f,
        const vector<ll>& g,
        int n,
        ll omega)
{
    //if (n == 1) return {(f[0] * g[0]) % MOD};
    if (n <= (1 << 7)) {
        auto full_prod = karatsuba(f, g);

        vector<ll> res(n, 0);
        for (int i = 0; i < full_prod.size(); i++)
        {
            res[i % n] = (res[i % n] + full_prod[i]) % MOD;
        }
        return res;
    }

    int m = n >> 1;

    vector<ll> f0(m), f1(m), g0(m), g1(m);
    ll curr_w = 1;

    for (int i = 0; i < m; ++i)
    {
        /*
        f0[i] = (f[i] + f[i + m]) % MOD; // x^m -1
        g0[i] = (g[i] + g[i + m]) % MOD;

        ll diff_f = (f[i] - f[i + m] + MOD) % MOD; // x^m + 1
        ll diff_g = (g[i] - g[i + m] + MOD) % MOD;
         */

        f0[i] = f[i] + f[i + m];
        if (f0[i] >= MOD) f0[i] -= MOD;

        g0[i] = g[i] + g[i + m];
        if (g0[i] >= MOD) g0[i] -= MOD;

        ll diff_f = f[i] - f[i + m];
        if (diff_f < 0) diff_f += MOD;

        ll diff_g = g[i] - g[i + m];
        if (diff_g < 0) diff_g += MOD;

        f1[i] = (diff_f * curr_w) % MOD;
        g1[i] = (diff_g * curr_w) % MOD;

        curr_w = (curr_w * omega) % MOD; // a_curr_w * w^curr_w
    }

    ll omega2 = power(omega, 2);
    auto h0 = fast_convolution_rec(f0, g0, m, omega2);
    auto h1 = fast_convolution_rec(f1, g1, m, omega2);

    vector<ll> res(n);
    curr_w = 1;
    for (int i = 0; i < m; ++i)
    {
        ll inv_curr_w = power(curr_w, MOD - 2); //Ferm theorem
        ll h1_x = (h1[i] * inv_curr_w) % MOD; // h(xw) -> h(x)

        res[i] = (h0[i] + h1_x) * INV2 % MOD;
        res[i + m] = (h0[i] - h1_x + MOD) * INV2 % MOD;

        curr_w = (curr_w * omega) % MOD;
    }
    return res;
}

vector<ll> multiply_cyclic(const vector<ll>& a, const vector<ll>& b)
{
    int sz = max(a.size(), b.size());
    int n = 1;
    while (n < 2 * sz - 1) n <<= 1;

    ll root = power(3, (MOD - 1) / n);

    vector<ll> fa(n, 0), fb(n, 0);
    copy(a.begin(), a.end(), fa.begin());
    copy(b.begin(), b.end(), fb.begin());

    return fast_convolution_rec(fa, fb, n, root);
}

void itask8_24()
{
    vector<ll> a = {1, 2, 3, 4};   // 1 + 2x + 3x² + 4x³
    vector<ll> b = {5, 6, 0, 0};   // 5 + 6x

    vector<ll> e = {1, 3, 2, 4, 1, 0, 0, 0};
    vector<ll> f = {1, 1, 1, 1, 0, 0, 0, 0};

    auto c = multiply_cyclic(a, b);
    auto g = multiply_cyclic(e, f);

    cout << "Result: ";
    for (auto x : c) cout << x << " ";
    cout << endl;

    cout << "Result: ";
    for (auto x : g) cout << x << " ";
    cout << endl;
}

void task30_1_7(int n_size)
{
    srand(time(nullptr));
    int max_val = 10 * n_size;

    set<int> setA, setB;

    while (setA.size() < n_size) setA.insert(rand() % (max_val + 1));
    while (setB.size() < n_size) setB.insert(rand() % (max_val + 1));

    int limit = 2*max_val + 1;

    int n = 1;
    while(n < limit) n <<= 1;

    vector<ll> fa(n, 0), fb(n, 0);
    for(int x : setA) fa[x] = 1;
    for(int x : setB) fb[x] = 1;

    auto start = chrono::high_resolution_clock::now();
    auto res = multiply_cyclic(fa, fb);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, std::milli> elapsed = end - start;

    cout << "\n" << string(35, '-') << endl;

    cout << "Resulting Cartesian Sums (C = A + B):" << endl;
    cout << string(35, '-') << endl;
    cout << "| " << setw(10) << "Sum Value" << " | " << setw(15) << "Occurrences" << " |" << endl;
    cout << string(35, '-') << endl;

    int total_unique_sums = 0;
    for (int i = 0; i < n; ++i) {
        if (res[i] > 0) {
            cout << "| " << setw(10) << i << " | " << setw(15) << res[i] << " |" << endl;
            total_unique_sums++;
        }
    }
    cout << string(35, '-') << endl;

    cout << "Statistics:" << endl;
    cout << " - Calculation time: " << fixed << setprecision(3) << elapsed.count() << " ms" << endl;
    cout << " - Unique sums found: " << total_unique_sums << endl;
    cout << " - Total pairs processed: " << (ll)n_size * n_size << endl;
}