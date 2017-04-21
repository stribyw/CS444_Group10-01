/*
 * Shannon Ernst, Tanner Cecchetti, W Keith Striby Jr
 * Assignment 1, The Producer-Consumer Problem
 * CS444 - Operating Systems II
 * April 19th, 2017
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "mt19937ar.h"

#define BUFFER_SIZE 32

/*
 *Establish locking thread variable to control buffer manipulation by
 *multiple consumers and producers.
 */
pthread_mutex_t lock;

/*
 *Global variables for buffer information on buffer amount, starting location,
 *and ending location.
 */
int buffer = 0;
int start = 0;
int end = 0;

/*
 *Structure for consumer value added to buffer and work (wait) time to consume.
 */
struct numbers {
        int val;
        int wait;
};

struct numbers list[BUFFER_SIZE];

/*
 *Randomly assigns the value going into the buffer and the wait periods for the
 *producer and consumer. If program is program is not ran on os-class the x86
 *instruction rdrand is used, else the Mersenne Twister is used.
 */
int rand_num(int min, int max)
{
        unsigned int eax;
        unsigned int ebx;
        unsigned int ecx;
        unsigned int edx;
        char vendor[13];

        eax = 0x01;
        __asm__ __volatile__ (
                "cpuid;"
                : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                : "a"(eax)
        );
        if (ecx & 0x40000000) {
                unsigned int value;
                __asm__ __volatile__ (
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

/*
 *Controls adding to the buffer. The value and wait period amount is randomly
 *assigned to the end of the list, these values are printed to screen, and the
 *new end is determined.
 */
void add_list()
{
        list[end].val = rand_num(0, 100);
        list[end].wait = rand_num(2, 10);
        printf("adding to list - val: %d, wait: %d\n", list[end].val,
                                                list[end].wait);
        end = (end + 1) % BUFFER_SIZE;
        buffer++;
}

/*
 *Controls the removal from buffer. The buffer count is reduced, the number
 *to be removed is assigned, the start is reassigned to the new starting
 *position so that the start is only removed, and number removed is returned.
 */
struct numbers remove_list()
{
        buffer--;
        struct numbers n = list[start];
        start = (start + 1) % BUFFER_SIZE;
        return n;
}

/*
 *Monitor thread for buffer, and prints buffer count. 
 */
void *monitor(void *ptr)
{
        while (1) {
                printf("buffer: %d\n", buffer);
                sleep(1);
        }
}

/*
 *The producer thread locks the mutex. If the buffer is less than 32
 *production is added to the buffer, then the mutex is unlocked. The producer
 *then returns to sleep.
 */
void *producer(void *ptr)
{
        while (1) {
                pthread_mutex_lock(&lock);
                if (buffer < BUFFER_SIZE)
                        add_list();
                pthread_mutex_unlock(&lock);
                sleep(rand_num(3, 8));
        }
}

/*
 *The consumer thread is capable of consuming as long as the buffer is greater
 *than zero. The mutex lock is blocked until consuming is possible. Then
 *consumption happens, the mutex lock is unblocked, and the consumer goes back
 *to sleep for the wait period. 
 */
void *consumer(void *ptr)
{
        while (1) {
                pthread_mutex_lock(&lock);
                if (buffer > 0) {
                        struct numbers n = remove_list();
                        pthread_mutex_unlock(&lock);
                        sleep(n.wait);
                        printf("val: %d\n", n.val);
                } else {
                        pthread_mutex_unlock(&lock);
                }
        }
}

/*
 *Establishes and controls the creation of producer, comsumer, and monitor
 *threads. Number of threads is input from command line argument. The use of
 *the mutex lock is initialized.
 */
int main(int argc, char *argv[])
{
        pthread_t prod;
        pthread_t con;
        pthread_t mon;
        int num_threads = atoi(argv[1]);

        printf("threads: %d\n", num_threads);
        pthread_mutex_init(&lock, NULL);
        for (int i = 0; i < num_threads / 2; i++) {
                pthread_create(&con, NULL, consumer, NULL);
                pthread_create(&prod, NULL, producer, NULL);
        }
        if (num_threads % 2 == 1)
                pthread_create(&con, NULL, consumer, NULL);
        pthread_create(&mon, NULL, monitor, NULL);
        pthread_join(con, NULL);
}
