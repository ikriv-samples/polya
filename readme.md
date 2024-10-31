# Pólya conjecture verifier in C++

This project that verifies (and, expectedly, disproves) the [Pólya Conjecture (Wikipedia)](https://en.wikipedia.org/wiki/P%C3%B3lya_conjecture).

This a relatively simple mathematical fact that holds true for **a lot** of numbers, the smallest counterexample being 906,150,257. This program finds this counterxample, and many more.

## The Conjecture

Each natural number can be represented as a product of prime factors. Let OF(n) be the quantity of numbers with odd number of prime factors in the range 1...n, and EF(n) be the quantity of numbers with even number of prime factors in the range 1...n. The conjecture is that OF(n) >= EF(n) for any n>2.

For the first 10 numbers 1,2,3,4,5,6,7,8,9,10, the number of prime factors is 0,1,1,2,1,2,1,3,2,2 respectively. The number 1 is considered to have zero prime factors. OF(n) and EF(n) counts for n=2,3,4,5,6,7,8,9,10 are 1:1, 2:1, 2:2, 3:2, 3:3, 4:3, 5:3, 5:4, and 5:5. We can keep counting, and OF(n) will be greater or equal to EF(n) for the first 900 million or so numbers.

## Disproof of the conjecture: the algorithm

The enclosed program in C++ uses Eratosthenes sieve method to calculate the number of prime factors for each n. If it finds a number for which EF(n) is greater than OF(n), it prints the number and the value of OF(n)-EF(n), which would be negative in this case. The first such number is 906150257:-1, which coincides with what Wikipedia says. The largest (by module) negative difference by is achieved at 906316571:-829, which also matches Wikipedia.

The program allocates array of max_n bytes (1GB by default), and for each prime _p_ (and powers of p) walks the array, increasing by one the number of factors for numbers divisible by p. So, initially all even numbers get 1 factor, then all numbers divisible by 4 get one additional factor, then all numbers divisible by 8, 16, 32, etc. Then we switch to numbers divisible by 3, 9, 81, ..., then numbers divisible by 5, 25, 625, ..., etc.

This method is surprisingly fast, I was able to get results in about 21 seconds on my computer to verify 1 billion numbers. It uses O(n) memory. It is not easy to calculate its time complexity, since it makes as many passes over the array as there are primes or powers of primes under n, but then the passes themselves are increasingly short, in a non-linear way.

My previous version, tagged "v2" ran for about 15 minutes using the same amount of memory, and one before that ("v1") took a few hours, but used no additional memory.

Current version is single-threaded and uses only one core. I was thinking of parallelizing it, but with 21 seconds run time it makes little sense.

1 byte per number accomodates up to 255 factors. We don't need more than that: the smallest possible factor is 2, and thus the smallest number with 256 factors is 2<sup>256</sup>, or ~10<sup>77</sup>, while we are dealing with numbers on the order of magnitude of 10<sup>10</sup> or less.

## Compiling

**Microsoft Visual Studio**: Open the .sln file and use `Release x64` configuration.

**GCC** Use the following command line:
`gcc --std c++20 -lstdc++ -o polya PolyaConjecture.cpp`

## Command line arguments

When run without any arguments the program tries to verify the hypothesis for the first 1 billion numbers.

The arguments format is:

    PolyaConjecture [options] [max_n]

max_n is the number to run the algorithm to, a 64 bit integer. Default is 1 billion.
The options are:

**-d or --debug:** show debugging information

**-? or --help:** show quick usage information

## Program Output

### stdout

For each number that violates the conjecture, the program prints the number, and the value of OF(n)-EF(n), e.g.

906150263:-5.

It also prints the total time used in the end.

If **--debug** option is used, the program prints OF(n)-EF(n) for all numbers, not just when it's negative. E.g.

    99:3

### stderr

The program prints progress information so the user knows it is working. It prints each prime under 100 (or a power thereof), since those tend to take longer time, and then prints a dot every 200,000 numbers.

## See also

https://ikriv.com/blog/?p=4545
