#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
 
int* primes; 
int* count;

/** Temporary holder to pass an arguemnt to a thread */
typedef struct {
    int start;
    int max;
    int num_threads;
} ArgStruct;

int cmpfunc ( const void* a, const void* b){
   return ( *(int*)a - *(int*)b );
}

// Print out an error message and exit.
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit(1);
}

/**
 * Thread function to calculates prime numbers 
 */
void* calculatePrime(void* param){
    // Cast param to ArgStruct and retreive min/max
    ArgStruct* astruct = (ArgStruct *) param;
    int min = astruct->start;
    int max = astruct->max;

    printf("min=%d max=%d numthreads=%d\n", min, max, astruct->num_threads);
    
    // Calculates all prime numbers between 1 and a max value
    for(int i = min; i <= max; i+= astruct->num_threads){
        //printf("%d\n", i);
        // If its 1 or 2 continue
        if( i == 1 || i == 2 ) continue;

        // If its divisible by 2 stop
        if( i % 2 == 0 ) continue;
        
        // Every prime falls between +-1 of a multiple of 6
        if( ((i - 1)%6 == 0) || ((i + 1)%6 == 0) ){
            double sq = sqrt( (double) i);

            // SQRT Test
            for(int j = 0; j < (*count) ; j++){
                
                // Divisible by another  prime
                if( i % primes[j] == 0 ) break;

                // Siev Test
                if( sq < primes[j] ){
                    primes[(*count)] = i;
                    (*count)++;
                    break;
                }
            }
        } else { 
            continue; 
        }
    }
    
    // free memory
    free(param);
    // return the array of primes
    return (void*)primes;
}

// The main method
int main( int argc, char *argv[] ) {
    pthread_t thread[8];
    count = malloc(sizeof(int));
    int max = 0 ; // max number to calculate primes up to
    *count = 0;   // count of prime nums
    int numThreads = 1; // number of threads
    
    // Check if we have correct args for ./prime [numThreads] [max]
    if(argc == 3){
    	numThreads = atoi(argv[1]);
        max = atoi(argv[2]); 
    } else {
        fail("Invalid args: ./prime [numThreads] [max]");
    }
   
    // Allocate array to hold prime numbers
    primes = (int*) malloc(sizeof(int) * (max) );
    
    // Given 2 since its special case
    primes[*count] = 2;
    (*count)++;
    // Given 3 since its special case 
    primes[*count] = 3;
    (*count)++;
    
    // Lets create threads
    for (int i = 0; i < numThreads; i++){
        // astruct to pass data to thread function
        ArgStruct *astruct = (ArgStruct *)malloc( sizeof( ArgStruct ) );
        astruct->start = i;
        astruct->max = max;
        astruct->num_threads = numThreads;
        
        // Create thread and send it to calculatePrime() with astruct data
        if(pthread_create(&thread[i], NULL, (void*)calculatePrime, astruct ) != 0 )
            fail("ERROR creating thread");
    }
    
    // Join threads so they finish together
    for(int i = 0; i < numThreads; i++)
        pthread_join( thread[i], NULL);
    
    // Sorts the prime numbers
    qsort(primes, *count, sizeof(int), cmpfunc);
    // Array printing loop
    //for(int i = 0; i < (*count); i++)
       // printf( "primes[%d] = %i\n", i, primes[i] );
    printf( "num primes = %d\n", *count);
    // Free memory used
    free(primes);
    free(count);

    return 0;
}

/**
First 100 primes
primes[0] = 2
primes[1] = 3
primes[2] = 5
primes[3] = 7
primes[4] = 11
primes[5] = 13
primes[6] = 17
primes[7] = 19
primes[8] = 23
primes[9] = 29
primes[10] = 31
primes[11] = 37
primes[12] = 41
primes[13] = 43
primes[14] = 47
primes[15] = 53
primes[16] = 59
primes[17] = 61
primes[18] = 67
primes[19] = 71
primes[20] = 73
primes[21] = 79
primes[22] = 83
primes[23] = 89
primes[24] = 97

*/
