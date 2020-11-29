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

    bool do_print_factors = false;
    bool do_print_primes = false;

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
        int max_prime_idx_to_consider = -1;
        int next_prime_square = 4;

        for (int n = 2; n <= total_numbers; ++n)
        {
            if (n >= next_prime_square)
            {
                ++max_prime_idx_to_consider;
                if (max_prime_idx_to_consider < primes.size() - 1)
                {
                    auto next_prime = primes[max_prime_idx_to_consider + 1];
                    next_prime_square = next_prime * next_prime;
                }
                else
                {
                    // n >= last prime squared, and it's the last item in the primes vector
                    // This means that either we stopped adding primes because we reached the last prime
                    // before sqrt(total_numbers), or that there were no primes between the last prime
                    // and the last prime squared. The latter, however, is impossible due to Bertrand postulate
                    // (https://en.wikipedia.org/wiki/Bertrand%27s_postulate). 
                    // So, there are no more primes worth considering. We avoid switching to the next prime
                    // by setting next_prime_square to a value exceeding total_numbers
                    next_prime_square = total_numbers + 1;
                }
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


