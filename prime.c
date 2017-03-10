/**
 * File - prime.c
 * This program calculates every prime number between 0 and a user-specified maximumm
 * @author Thomas Ortiz (tdortiz@ncsu.edu)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
 
/** An array of primes */
int* primes; 
/** The length of the array of primes */
int* count;
/** A semaphore for the multiple threads */
sem_t sem;

/** Temporary holder to pass an arguemnt to a thread */
typedef struct {
    int start; // starting point
    int max;   // ending point
} ArgStruct;

/**
 * Used with qsort when debugging.
 * @param a - an int
 * @param b - an int
 * @return 0 if equal, < 0 if a comes before, > 0 if a comes after
 */
int cmpfunc ( const void* a, const void* b){
   return ( *(int*)a - *(int*)b );
}

/**
 * Print out an error message and exits the program.
 * @param message to send to stdout 
 */
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit(1);
}

/**
 * Thread function to calculates prime numbers 
 * @param param - state information for the specific thread
 */
void* calculatePrime(void* param){
    // Cast param to ArgStruct and retreive min/max
    ArgStruct* astruct = (ArgStruct *) param;
    int min = astruct->start;
    int max = astruct->max;

    printf("%8s%d\t%8s%d\t\n","Min=", min, "Max=", max);
    
    // Calculates all prime numbers between 1 and a max value
    for(int i = min; i <= max; i++){
        // If its 1 or 2 or even,  continue
        if( i == 1 || i == 2 || i % 2 == 0 ) continue;
        
        // Every prime falls between +-1 of a multiple of 6
        if( ((i - 1)%6 == 0) || ((i + 1)%6 == 0) ){
            double sq = sqrt( (double) i);
            bool prime = true;
            
            // SQRT Test 
            for( int j = 2; j <= sq; j++ ){       
                if( i % j == 0 ) prime = false;
            }
		
            // If its a prime add it to the list
            if( prime ){
                sem_wait(&sem);
                primes[(*count)] = i;
                (*count)++;
                sem_post(&sem);
            }
        }
    }
    
    // free memory
    free(param);
    // return the array of primes
    return (void*)primes;
}

/**
 * Sorts the prime numbers array and prints it out
 */
void sortAndPrint(){
    qsort(primes, *count, sizeof(int), cmpfunc);
    // Array printing loop
    for(int i = 0; i < (*count); i++)
        printf( "primes[%d] = %i\n", i, primes[i] );
}

/**
 * The main method
 * @param argc count of commandline args
 * @param argv[] the array of args
 */
int main( int argc, char *argv[] ) {
    pthread_t thread[8];
    count = malloc(sizeof(int));
    int max = 0 ;       // max number to calculate primes up to
    *count = 0;         // count of prime nums
    int numThreads = 1; // number of threads
    sem_init(&sem, 0, 1);
    
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

        // calculate where to start
        astruct->start = (max/numThreads)*i + 1;

        // calculate where to end
        int threadMax = max/numThreads*(i+1);
        if( i == numThreads-1 ){
            threadMax = max;
        }
        astruct->max = threadMax;
        
        // Create thread and send it to calculatePrime() with astruct data
        if(pthread_create(&thread[i], NULL, (void*)calculatePrime, astruct ) != 0 )
            fail("ERROR creating thread");
    }
    
    // Join threads so they finish together
    for(int i = 0; i < numThreads; i++)
        pthread_join( thread[i], NULL);
    
    //sortAndPrint();
    printf( "num primes = %d\n", *count);
    
    // Free memory used
    free(primes);
    free(count);

    return 0;
}
