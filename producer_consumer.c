/*
 *Shannon Ernst, Tanner Cecchetti, W Keith Striby Jr
 *Assignment 1, The Producer-Consumer Problem
 *
 *
 *
 */

#include <stdio.h>
#include <pthread.h>

pthread_t prod_con;
//pthread_mutex_t lock;
pthread_cond_t con_cond, prod_cond;
int buffer = 32;

struct numbers

void* producer(void* ptr){

    int i;

    while(true){
        pthread_mutex_lock(&prod_con);      //protect buffer
        while(buffer == 32)
            pthread_cond_wait(&prod_cond, &prod_con);
        buffer++;
        pthread_cond_signal(con_cond);
        pthread_mutex_unlock(&prod_con);
    }
    pthread_exit(0);
}

void* consumer(void* ptr){

    int i;

    while(true){
        pthread_mutex_lock(&prod_con);      //protect buffer
        while(buffer == 0)
            pthread_cond_wait(&con_cond, & prod_con);
        buffer--;
        pthread_cond_signal(&prod_cond);        //wake up producer
        pthread_mutex_unlock(&prod_con);
    }
    pthread_exit(0);
}

int main(int argc, char* argv[]){
    
    pthread_t prod;
    pthread_t con;
    num_threads = argv[1];

    /*
     *initialize mutex and condition variables
     *
     */
    pthread_mutex_init(&the_mutex, NULL);
    pthread_cond_init(&con_cond, NULL);         //initialize consumer condition
    pthread_cond_init(&prod_con, NULL);         //initialize producer condition

    //create threads
    pthread_create(&con, NULL, consumer, NULL);
    pthread_create(&prod, NULL, producer, NULL);

    //wait for other thread to finish
    pthread_join(&con, NULL);
    pthread_join(&prod, NULL);

}
