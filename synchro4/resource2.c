#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t thread_count_lock;
int count_threads;

pthread_mutex_t count_in_use_lock;
int count_in_use;

pthread_cond_t cond_cooled_down;
int on_cool_down;

void *print_thread(void *ptr)
{
   while(1) {
      printf("\n\n\n");
      printf("total threads: %d\n", count_threads);
      printf("threads using resource: %d\n", count_in_use);
      printf("on cool down: %s\n", on_cool_down ? "yes" : "no");
      sleep(1);
   }
}

void *locker(void *ptr)
{
   pthread_mutex_lock(&thread_count_lock);
   count_threads++;
   pthread_mutex_unlock(&thread_count_lock);

   pthread_mutex_lock(&count_in_use_lock);
   while(on_cool_down && count_in_use > 0)
      pthread_cond_wait(&cond_cooled_down, &count_in_use_lock);

   count_in_use++;
   if(count_in_use == 3)
      on_cool_down = 1;
   pthread_mutex_unlock(&count_in_use_lock);

   //printf("locker started\n");
   sleep(rand() % 4 + 4);
   //printf("locked finished\n");

   pthread_mutex_lock(&count_in_use_lock);
   count_in_use--;
   if(on_cool_down && !count_in_use) {
      pthread_cond_signal(&cond_cooled_down);
      on_cool_down = 0;
   }
   pthread_mutex_unlock(&count_in_use_lock);

   pthread_exit(0);
}

int main(int argc, char *argv[])
{
   if(argc != 1) {
      printf("usage: ./a.out\n");
      exit(0);
   }

   pthread_mutex_init(&count_in_use_lock, NULL);
   count_in_use = 0;

   pthread_mutex_init(&thread_count_lock, NULL);
   count_threads = 0;

   pthread_cond_init(&cond_cooled_down, NULL);
   on_cool_down = 0;

   pthread_t t;
   pthread_create(&t, NULL, print_thread, NULL);
   while(1) {
      pthread_create(&t, NULL, locker, NULL);
      sleep(rand() % 4);
   }
}
