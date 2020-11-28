// PolyaConjecture.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

vector<int> primes;

static int get_num_prime_factors(int n, int ceil_sqrt_n)
{
    int factors = 0;
    for (auto prime: primes)
    {
        if (prime > n || prime > ceil_sqrt_n) break;

        while (n % prime == 0)
        {
            n /= prime;
            ++factors;
        }
    }

    if (!factors)
    {
        primes.push_back(n);
        return 1;
    }

    if (n != 1)
    {
        // account for the remaining prime factor greater than sqrt(n)
        // E.g. if n=10, such factor would be 5
        ++factors;
    }

    return factors;
}

int main()
{
    int ceil_sqrt_n = 1;
    int ceil_sqrt_n_square = 1;
    int odd_factored_numbers_found = 0; // consider 1 to have even number of prime factors (zero)

    for (int n = 2; n < 10000000000; ++n)
    {
        if (ceil_sqrt_n_square < n)
        {
            ++ceil_sqrt_n;
            ceil_sqrt_n_square = ceil_sqrt_n * ceil_sqrt_n;
        }

        auto factors = get_num_prime_factors(n, ceil_sqrt_n);
        if (factors % 2 != 0)
        {
            ++odd_factored_numbers_found;
        }

        // print progress
        if (n % 200000 == 0) cout << ".";
        if (n % 10000000 == 0) cout << endl;

        auto even_factored_numbers_found = n - odd_factored_numbers_found;
        if (even_factored_numbers_found > odd_factored_numbers_found)
        {
            cout << endl << n << ":" << even_factored_numbers_found-odd_factored_numbers_found << endl;
        }
    }
}


