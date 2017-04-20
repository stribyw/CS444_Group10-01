/*
 *Shannon Ernst, Tanner Cecchetti, W Keith Striby Jr
 *Assignment 1, The Producer-Consumer Problem
 *
 *
 *
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

pthread_t prod_con;
pthread_mutex_t lock;

int buffer = 0;

struct numbers {
   int val;
   int wait;
};

struct numbers list[32];

#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

/* initializes mt[N] with a seed */
void init_genrand(unsigned long s)
{
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<N; mti++) {
        mt[mti] =
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }

    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

int rand_num(int min, int max) {
   unsigned int eax;
   unsigned int ebx;
   unsigned int ecx;
   unsigned int edx;

   char vendor[13];

   eax = 0x01;

   __asm__ __volatile__(
                        "cpuid;"
                        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                        : "a"(eax)
                        );

   if(ecx & 0x40000000){
      unsigned int value;
      __asm__ __volatile__(
                           "rdrand %[value];"
                           : [value] "=r" (value)
                           :
                           : "cc"
                           );
      return value % (max - min) + min;
   } else {
      return genrand_int32() % (max - min) + min;
   }
}

void add_list() {
   list[buffer].val = rand_num(0, 100);
   list[buffer].wait = rand_num(2, 10);
   printf("adding to list - val: %d, wait: %d\n", list[buffer].val, list[buffer].wait);
   buffer++;
}

struct numbers remove_list() {
   buffer--;
   return list[buffer];
}

void* monitor(void* ptr) {
   while(1) {
      printf("buffer: %d\n", buffer);
      sleep(1);
   }
}

void* producer(void* ptr){
   int i;

   while(1) {
        pthread_mutex_lock(&lock);      //protect buffer
        if(buffer < 32) {
           add_list();
        }
        pthread_mutex_unlock(&lock);
        sleep(rand_num(3, 8));
    }
    //pthread_exit(0);
}

void* consumer(void* ptr){
   while(1) {
        pthread_mutex_lock(&lock);      //protect buffer
        if(buffer > 0) {
           struct numbers n = remove_list();
           pthread_mutex_unlock(&lock);

           sleep(n.wait);
           printf("val: %d\n", n.val);
        } else {
           pthread_mutex_unlock(&lock);
        }
    }
    //pthread_exit(0);
}

int main(int argc, char* argv[]){
    pthread_t prod;
    pthread_t con;
    pthread_t mon;
    int num_threads = atoi(argv[1]);

    printf("threads: %d\n", num_threads);

    /*
     *initialize mutex and condition variables
     *
     */
    pthread_mutex_init(&lock, NULL);

   //create threads
   for(int i = 0; i < num_threads / 2; i++) {
      pthread_create(&con, NULL, consumer, NULL);
      pthread_create(&prod, NULL, producer, NULL);
   }
   if(num_threads % 2 == 1) {
      pthread_create(&con, NULL, consumer, NULL);
   }

   pthread_create(&mon, NULL, monitor, NULL);

    //wait for other thread to finish
    pthread_join(con, NULL);
    //pthread_join(&prod, NULL);
}
