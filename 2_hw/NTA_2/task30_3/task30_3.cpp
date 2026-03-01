//
// Created by mvideo on 28/02/26.
//
#include"task30_3.hpp"

using namespace std;
using ll = long long;

vector<ll> ntt_only(
        const vector<ll>& f,
        ll omega)
{
    int n = f.size();
    if (n == 1) return f;

    int m = n >> 1;
    vector<ll> f0(m), f1(m);
    ll curr_w = 1;

    for (int i = 0; i < m; ++i)
    {
        /*
        f0[i] = (f[i] + f[i + m]) % MOD;
        ll diff = (f[i] - f[i + m] + MOD) % MOD;
        f1[i] = (diff * curr_w) % MOD;

        curr_w = (curr_w * omega) % MOD;
         */
        f0[i] = f[2 * i];
        f1[i] = f[2*i+1];
    }

    ll omega2 = power(omega, 2);
    auto h0 = ntt_only(f0, omega2);
    auto h1 = ntt_only(f1, omega2);

    vector<ll> res(n);
    curr_w = 1;
    for (int i = 0; i < m; ++i)
    {
        /*
        res[2 * i] = h0[i];
        res[2 * i + 1] = h1[i];
         */

        ll t = (curr_w * h1[i]) % MOD;
        res[i] = (h0[i] + t) % MOD;
        res[i + m] = (h0[i] - t + MOD) % MOD;

        curr_w = (curr_w * omega) % MOD;
    }
    return res;
}

void multidimensional_dft(vector<ll>& A,
                          const vector<int>& dimensions)
{
    int total_n = A.size();
    int distance = 1;

    for(int n_i : dimensions)
    {
        int num_blocks = total_n / (n_i * distance);
        for (int b = 0; b < num_blocks; ++b)
        {
            for(int s = 0; s < distance; s++)
            {
                vector<ll> strip(n_i);
                int start_idx = b * (n_i * distance) + s;
                for(int k = 0; k < n_i; ++k)
                {
                    strip[k] = A[start_idx + k * distance];
                }

                ll root = power(3, (MOD - 1) / n_i);
                vector<ll> transformed = ntt_only(strip, root);

                for (int k = 0; k < n_i; ++k) {
                    A[start_idx + k * distance] = transformed[k];
                }
            }
        }
        distance *= n_i;
    }
}

void print_task_30_3(
        const string& title,
        const vector<ll>& data,
        const vector<int>& dims
        )
{
    cout << "\n=== " << title << " ===\n";

    if (dims.size() == 2)
    {
        int rows = dims[0];
        int cols = dims[1];
        cout << string(cols * 12 + 5, '-') << endl;
        for (int i = 0; i < rows; ++i) {
            cout << "| ";
            for (int j = 0; j < cols; ++j) {
                cout << setw(10) << data[i * cols + j] << " | ";
            }
            cout << endl;
        }
        cout << string(cols * 12 + 5, '-') << endl;
    }
    else {
        for (int i = 0; i < data.size(); ++i) {
            cout << "Index [" << i << "]: " << data[i] << endl;
        }
    }
}

void task30_3()
{
    vector<int> dims = {4, 4};
    int total_n = 1;
    for(int d : dims) total_n *= d;

    vector<ll> data(16);
    for(int i=0; i<16; ++i) data[i] = i + 1;

    print_task_30_3("INPUT DATA (A)", data, dims);

    auto start = chrono::high_resolution_clock::now();

    multidimensional_dft(data, dims);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> elapsed = end - start;

    print_task_30_3("OUTPUT DFT (Y)", data, dims);

    cout << "Calculation time: " << fixed << setprecision(3) << elapsed.count() << " ms" << endl;
    cout << "Total elements processed: " << total_n << " (independent of d=" << dims.size() << ")" << endl;
}