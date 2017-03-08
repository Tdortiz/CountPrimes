# CountPrimes
This is a multithreaded program that is used to quickly calculate all of the primes between 0 and a max number. In the program I utilize semaphores to safely lock threads and ensure no unsafe data interactions.


This program is ran by compiling like so:

    gcc -Wall -std=c99 prime.c -lm -pthread -o prime
    
and ran by 

    ./prime [num threads] [maxNumber]

It uses two rules to determine if a number is prime:

    1. Every prime (except 2 & 3) is withing +- of a multiple of 6
    
    2. SQRT theory - When checking factors for number 'x' you only have to check from 2 to sqrt(x). 
    * This cuts down on A LOT of processing. 
    * For example, to check if 1,000,000 is a prime (ignoring a even number check) you would just have to check 
      for factors between 1 and 1,000 instead of 1 to 1,000,000
