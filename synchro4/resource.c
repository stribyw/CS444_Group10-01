#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t print_lock;

pthread_mutex_t thread_count_lock;
int count_threads;
int count_finished;

pthread_mutex_t count_in_use_lock;
int count_in_use;

pthread_cond_t cond_cooled_down;
int on_cool_down;

void print_state()
{
        pthread_mutex_lock(&print_lock);
        printf("\n\n\n");
        printf("threads:\n");
        printf("  total: %d\n", count_threads);
        printf("  waiting: %d\n", count_threads - count_finished - count_in_use);
        printf("  running: %d\n", count_in_use);
        printf("  finished: %d\n", count_finished);
        printf("on cool down: %s\n", on_cool_down ? "yes" : "no");
        pthread_mutex_unlock(&print_lock);
}

void *locker(void *ptr)
{
        pthread_mutex_lock(&count_in_use_lock);
        count_threads++;
        while(on_cool_down && count_in_use > 0)
                pthread_cond_wait(&cond_cooled_down, &count_in_use_lock);

        count_in_use++;
        if(count_in_use == 3)
                on_cool_down = 1;

        print_state();
        pthread_mutex_unlock(&count_in_use_lock);

        sleep(rand() % 4 + 2);

        pthread_mutex_lock(&count_in_use_lock);
        count_finished++;
        count_in_use--;
        if(on_cool_down && !count_in_use) {
                pthread_cond_signal(&cond_cooled_down);
                on_cool_down = 0;
        }
        print_state();
        pthread_mutex_unlock(&count_in_use_lock);

        pthread_exit(0);
}

int main(int argc, char *argv[])
{
        if(argc != 1) {
                printf("usage: ./a.out\n");
                exit(0);
        }

        pthread_mutex_init(&print_lock, NULL);

        pthread_mutex_init(&count_in_use_lock, NULL);
        count_in_use = 0;
        count_threads = 0;
        count_finished = 0;

        pthread_cond_init(&cond_cooled_down, NULL);
        on_cool_down = 0;

        pthread_t t;
        while(1) {
                pthread_create(&t, NULL, locker, NULL);
                sleep(rand() % 2);
        }
}
