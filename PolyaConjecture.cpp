// PolyaConjecture.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

const int TOTAL_NUMBERS = 1000 * 1000 * 1000; // 1 billion
const int MAX_PRIME_TO_ADD = int(sqrt(TOTAL_NUMBERS)+1e-5);

vector<int> primes;
vector<short> num_factors = { 0 };
int even_factors = 1; // total numbers with even number of factors seen so far

static short calc_num_prime_factors(int n, int max_prime_idx_to_consider)
{
    assert((int)num_factors.size() == n - 1);
    for (int prime_idx = 0; prime_idx<=max_prime_idx_to_consider; ++prime_idx)
    {
        auto prime = primes[prime_idx];
        if (n % prime == 0)
        {
            auto factors = num_factors[n / prime - 1] + 1;
            num_factors.push_back(factors);
            if (factors % 2 == 0) ++even_factors;
            return factors;
        }
    }

    // no factors: n is prime
    num_factors.push_back(1);
    if (n <= MAX_PRIME_TO_ADD) primes.push_back(n);
    return 1;
}

int main()
{
    int floor_sqrt_n = 1;
    int next_square = (floor_sqrt_n + 1) * (floor_sqrt_n + 1);
    int max_prime_to_consider = 1;
    int max_prime_idx_to_consider = -1;

    for (int n = 2; n <= TOTAL_NUMBERS; ++n)
    {
        if (n >= next_square)
        {
            ++floor_sqrt_n;
            next_square = (floor_sqrt_n + 1) * (floor_sqrt_n + 1);
        }

        if (max_prime_to_consider < floor_sqrt_n && next_square <= TOTAL_NUMBERS)
        {
            ++max_prime_idx_to_consider;
            max_prime_to_consider = primes[max_prime_idx_to_consider];
        }

        auto factors = calc_num_prime_factors(n, max_prime_idx_to_consider);

        // print progress
        if (n % 200000 == 0) cout << ".";
        if (n % 10000000 == 0) cout << endl << n;

        auto odd_factors = n - even_factors;
        // cout << n << ":" << factors << ":" << odd_factors << ":" << even_factors << endl;

        if (even_factors > odd_factors)
        {
            cout << endl << n << ":" << even_factors-odd_factors << endl;
        }
    }

    for (auto prime : primes)
        cout << prime << " ";
}


