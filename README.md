# CountPrimes
This program is ran by compiling like so:

    gcc -Wall -std=c99 prime.c -lm -pthread -o prime
    
and ran by 

    ./prime [num threads] [maxNumber]

It uses two rules to determine if a number is prime:

    1. Every prime (except 2 & 3) is withing +-1 of a multiple of 6
    
    2. Sieve of Eratosthenes
