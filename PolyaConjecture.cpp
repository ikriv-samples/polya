// PolyaConjecture.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <assert.h>
#include <chrono>

using namespace std;

class PolyaCalculator
{
    int total_numbers;
    int max_prime_to_add;
    vector<int> primes;
    vector<short> num_factors = { 0 };
    int even_factors = 1;

    bool do_print_factors;
    bool do_print_primes;

public:
    explicit PolyaCalculator(int total_numbers)
        :
        total_numbers(total_numbers),
        max_prime_to_add(int(sqrt(total_numbers) + 1e-5))
    {
    }

    void print_factors() { do_print_factors = true; }
    void print_primes() { do_print_primes = true; }

    short calc_num_prime_factors(int n, int max_prime_idx_to_consider)
    {
        assert((int)num_factors.size() == n - 1);
        for (int prime_idx = 0; prime_idx <= max_prime_idx_to_consider; ++prime_idx)
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
        if (n <= max_prime_to_add) primes.push_back(n);
        return 1;
    }

    void run()
    {
        int floor_sqrt_n = 1;
        int next_square = (floor_sqrt_n + 1) * (floor_sqrt_n + 1);
        int max_prime_to_consider = 1;
        int max_prime_idx_to_consider = -1;

        for (int n = 2; n <= total_numbers; ++n)
        {
            if (n >= next_square)
            {
                ++floor_sqrt_n;
                next_square = (floor_sqrt_n + 1) * (floor_sqrt_n + 1);
            }

            if (max_prime_to_consider < floor_sqrt_n && next_square <= total_numbers)
            {
                ++max_prime_idx_to_consider;
                max_prime_to_consider = primes[max_prime_idx_to_consider];
            }

            auto factors = calc_num_prime_factors(n, max_prime_idx_to_consider);

            // print progress
            if (n % 200000 == 0) cerr << ".";
            if (n % 10000000 == 0) cerr << endl << n;

            auto odd_factors = n - even_factors;

            if (do_print_factors)
            {
                cout << n << ":" << factors << ":" << odd_factors << ":" << even_factors << endl;
            }
            else if (even_factors > odd_factors)
            {
                cout << n << ":" << even_factors - odd_factors << endl;
            }
        }
        cout << endl;
        cerr << endl;

        if (do_print_primes)
        {
            for (auto prime : primes)
            {
                cout << prime << " ";
            }
            cout << endl;
        }
    }
};

void timed_run(PolyaCalculator& calc)
{
    typedef chrono::high_resolution_clock Clock;
    auto start = Clock::now();
    calc.run();
    auto end = Clock::now();

    chrono::duration<double, std::ratio<1, 1>> seconds = end - start;
    cout << "Total time: " << seconds.count() << "s" << endl;
}

int main(int argc, char** argv)
{
    int max_n = 1000 * 1000 * 1000;
    if (argc >= 2)
    {
        max_n = atoi(argv[1]);
    }

    bool debug = false;
    if (argc >= 3 && argv[2] == std::string("--debug"))
    {
        debug = true;
    }

    auto calc = PolyaCalculator(max_n);
    if (debug)
    {
        calc.print_factors();
        calc.print_primes();
    }

    timed_run(calc);
    return 0;
}


