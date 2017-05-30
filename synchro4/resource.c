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
pthread_mutex_t print_lock;

struct resource {
        int count;
};

struct resource r;

/*void *get_resource(void *ptr)
{
        //while(1){
                pthread_mutex_lock(&first_lock);
                printf("Resource has first user.\n");
                r.count++;
                sleep(5);
                
                pthread_mutex_lock(&second_lock);
                printf("Resource has second user.\n");
                r.count++;
                sleep(5);
                
                pthread_mutex_lock(&third_lock);
                printf("Resource has third user.\n");
                r.count++;
                sleep(5);

                pthread_mutex_unlock(&third_lock);
                printf("Resource third user done.\n");
                r.count--;

                pthread_mutex_unlock(&second_lock);
                printf("Resource second user done.\n");
                r.count--;

                pthread_mutex_unlock(&first_lock);
                printf("Resource first user done.\n");
                r.count--;
        //}
}*/

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
        sleep(3);
        printf("third lock not in use\n");
        r.count--;
        pthread_mutex_unlock(&third_lock);
}

void* second(void* ptr)
{
        pthread_mutex_lock(&second_lock);
        r.count++;
        printf("second lock in use\n");
        sleep(3);
        printf("second lock not in use\n");
        r.count--;
        pthread_mutex_unlock(&second_lock);
}

void* first(void* ptr)
{
        printf("in first\n");
        pthread_mutex_lock(&first_lock);
        r.count++;
        printf("first lock in use\n");
        sleep(3);
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
        pthread_t thread;

        int try1 = 0;
        int try2 = 0;
        int try3 = 0;

        time_t t;
        srand((unsigned) time (&t));

        while(1) {
                //if(r.count <= 3) {
                        try1 = pthread_mutex_trylock(&first_lock);
                        printf("try1: %d\n", try1);
                        if(try1 != 0) {
                                printf("in if try1\n");
                                pthread_create(&thread, NULL, first, NULL);
                                print_status();
                                sleep(2);
                                print_status();
                                //try1 = -5;
                        }
                //} else if(r.count <= 3) {
                        try2 = pthread_mutex_trylock(&second_lock);
                        if(try2 != 0) {
                                pthread_create(&thread, NULL, second, NULL); 
                                print_status();
                                sleep(2);
                                print_status();
                                //try2 = -5;
                        }
                //} else if(r.count <= 3) {
                        try3 = pthread_mutex_trylock(&third_lock);
                        if(try3 != 0) {
                                pthread_create(&thread, NULL, third, NULL); 
                                print_status();
                                sleep(2);
                                print_status();
                                //try3 = -5;
                        }
                //} else {
                        printf("thread blocked\n");
                        print_status();
                //}
                sleep(3);
        }
}
