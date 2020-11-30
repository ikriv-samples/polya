// PolyaConjecture.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <assert.h>
#include <chrono>

using namespace std;
typedef int64_t number;

class PolyaCalculator
{
    number total_numbers;
    vector<uint8_t> num_factors;

    bool do_print_factors = false;
    bool do_print_primes = false;

public:
    explicit PolyaCalculator(number total_numbers)
        :
        total_numbers(total_numbers),
        num_factors(total_numbers+1, 0)
    {
        cerr << "Allocated memory" << endl;
    }

    void print_factors() { do_print_factors = true; }
    void print_primes() { do_print_primes = true; }

    static number safe_multiply(number prime_power, number prime, number limit)
    {
        if (limit / prime_power < prime) return limit + 1;
        return prime_power * prime;
    }

    void run()
    {
        number count = 0;

        for (number prime = 2; prime <= total_numbers; ++prime)
        {
            if (prime % 200000 == 0) cerr << ".";
            if (prime % 10000000 == 0) cerr << endl;

            if (num_factors[prime] != 0) continue; // not prime

            for (number prime_power = prime; prime_power <= total_numbers; prime_power = safe_multiply(prime_power, prime, total_numbers))
            {
                if (prime_power < 100)
                {
                    cerr << prime_power << " ";
                }

                for (number n = prime_power; n < total_numbers; n += prime_power)
                {
                    ++num_factors[n];
                }
            }
        }

        number even_factors = 0;

        for (number n = 2; n < total_numbers; ++n)
        {
            auto factors = num_factors[n];
            if (factors % 2 == 0)
            {
                ++even_factors;
            }

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
    number max_n = 1000 * 1000 * 1000;

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
