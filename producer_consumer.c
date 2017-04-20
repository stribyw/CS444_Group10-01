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
//thread_cond_t con_cond, prod_cond;

int buffer = 0;

struct numbers {
   int val;
   int wait;
};

struct numbers list[32];

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

           buffer++;
        }
        pthread_mutex_unlock(&lock);
    }
    //pthread_exit(0);
}

void* consumer(void* ptr){
   while(1) {
        pthread_mutex_lock(&lock);      //protect buffer
        if(buffer > 0) {
           buffer--;
        }
        pthread_mutex_unlock(&lock);
    }
    //pthread_exit(0);
}

int main(int argc, char* argv[]){
    pthread_t prod;
    pthread_t con;
    pthread_t mon;
    int num_threads = atoi(argv[1]);
    //int i;

    printf("threads: %d\n", num_threads);

    /*
     *initialize mutex and condition variables
     *
     */
    pthread_mutex_init(&lock, NULL);
   //  pthread_cond_init(&con_cond, NULL);         //initialize consumer condition
   //  pthread_cond_init(&prod_con, NULL);         //initialize producer condition

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
