/*
 *
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t first_lock;
pthread_mutex_t second_lock;
pthread_mutex_t third_lock;
pthread_mutex_t all_lock;
pthread_mutex_t print_lock;

struct resource {
        int count;
};

struct resource r;


void print_status()
{
        pthread_mutex_lock(&print_lock);
        printf("Number of resources in use: %d.\n", r.count);
        pthread_mutex_unlock(&print_lock);
}

void* third(void* ptr)
{
        pthread_mutex_lock(&third_lock);
        r.count++;
        printf("third lock in use\n");
        sleep(4);
        printf("third lock not in use\n");
        r.count--;
        pthread_mutex_unlock(&third_lock);
}

void* second(void* ptr)
{
        pthread_mutex_lock(&second_lock);
        r.count++;
        printf("second lock in use\n");
        sleep(4);
        printf("second lock not in use\n");
        r.count--;
        pthread_mutex_unlock(&second_lock);
}

void* first(void* ptr)
{
        pthread_mutex_lock(&first_lock);
        r.count++;
        printf("first lock in use\n");
        sleep(4);
        printf("first lock not in use\n");
        r.count--;
        pthread_mutex_unlock(&first_lock);
}

//void make_thread()
int main()
{
        pthread_mutex_init(&first_lock, NULL);
        pthread_mutex_init(&second_lock, NULL);
        pthread_mutex_init(&third_lock, NULL);
        pthread_mutex_init(&all_lock, NULL);
        pthread_t thread;

        int choice = 0;
        time_t t;
        srand((unsigned) time (&t));

        while(1) {
                // if (r.count == 0) {
                //         pthread_mutex_unlock(&all_lock);
                        choice = rand () % 3 + 1;
                        if (choice == 1) {
                                print_status();
                                pthread_create(&thread, NULL, first, NULL);
                                print_status();
                        } else if (choice == 2) {
                                print_status();
                                pthread_create(&thread, NULL, second, NULL); 
                                print_status();
                        } else if (choice == 3) {
                                print_status();
                                pthread_create(&thread, NULL, third, NULL); 
                                print_status();
                        }
                // if (r.count == 3) {
                //         pthread_mutex_lock(&all_lock);        
        }
}
