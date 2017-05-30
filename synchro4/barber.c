#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define STATE_EMPTY 0
#define STATE_WAITING 1
#define STATE_HAIRCUT 2

#define BARBER_SLEEPING 0
#define BARBER_AWAKE 1

int num_chairs;
int filled_chairs;
int barber_state;
int *chair_states;
int customer_sleep_offset;

int count_customers_serviced;
int count_customers_rejected;

pthread_mutex_t print_lock;
pthread_mutex_t count_lock;
pthread_mutex_t *chair_locks;
pthread_cond_t *haircut_conds;
pthread_cond_t barber_wake_cond;

void print_chair_states()
{
        for(int i = 0; i < num_chairs; i++) {
                printf("chair %d: ", i);
                switch(chair_states[i]) {
                        case STATE_EMPTY: printf("empty\n"); break;
                        case STATE_WAITING: printf("customer waiting\n"); break;
                        case STATE_HAIRCUT: printf("getting haircut\n"); break;
                        default: printf("ERROR: UNKNOWN STATE\n"); break;
                }
        }
}

void print_state()
{
        printf("[barber] state: %s\n", barber_state == BARBER_SLEEPING ? "sleeping" : "awake");
        printf("[customers] total: %d | serviced: %d | rejected: %d\n",
        count_customers_serviced + count_customers_rejected,
        count_customers_serviced,
        count_customers_rejected);
        print_chair_states();
}

void *barber(void *ptr)
{
        while(1) {
                // Check if nobody is here, go to sleep
                pthread_mutex_lock(&count_lock);
                if(!filled_chairs) {
                        barber_state = BARBER_SLEEPING;
                        pthread_cond_wait(&barber_wake_cond, &count_lock);
                }
                pthread_mutex_unlock(&count_lock);

                // Barber is awake
                barber_state = BARBER_AWAKE;

                for(int i = 0; i < num_chairs; i++) {
                        // Check if chair contains a waiting customer
                        if(chair_states[i] == STATE_WAITING) {
                                // Start haircut, signal to customer it has begun
                                pthread_mutex_lock(&chair_locks[i]);
                                chair_states[i] = STATE_HAIRCUT;
                                pthread_cond_signal(&haircut_conds[i]);
                                pthread_mutex_unlock(&chair_locks[i]);

                                // Complete haircut
                                sleep(rand() % 4 + 2);

                                // End haircut, signal to customer it has ended
                                pthread_mutex_lock(&chair_locks[i]);
                                pthread_cond_signal(&haircut_conds[i]);
                                pthread_mutex_unlock(&chair_locks[i]);
                        }
                }
        }
}

void *customer(void *ptr)
{
        int index = -1;

        // Attempt to find an empty chair
        for(int i = 0; i < num_chairs; i++) {
                pthread_mutex_lock(&chair_locks[i]);
                if(chair_states[i] == STATE_EMPTY) {
                        index = i;
                        break;
                }
                pthread_mutex_unlock(&chair_locks[i]);
        }

        // Unable to find a chair
        if(index == -1) {
                pthread_mutex_lock(&count_lock);
                count_customers_rejected++;
                pthread_mutex_unlock(&count_lock);

                pthread_exit(0);
        }

        // Increment filled chair count
        pthread_mutex_lock(&count_lock);
        filled_chairs++;
        pthread_cond_signal(&barber_wake_cond);
        pthread_mutex_unlock(&count_lock);

        // Wait for haircut to begin
        chair_states[index] = STATE_WAITING;
        pthread_cond_wait(&haircut_conds[index], &chair_locks[index]);

        // Wait for haircut to finish
        pthread_cond_wait(&haircut_conds[index], &chair_locks[index]);

        // Leave the shop
        chair_states[index] = STATE_EMPTY;

        pthread_mutex_lock(&count_lock);
        count_customers_serviced++;
        filled_chairs--;
        pthread_mutex_unlock(&count_lock);

        pthread_mutex_unlock(&chair_locks[index]);

        pthread_exit(0);
}

void *print_thread(void *ptr)
{
        while(1) {
                printf("\n\n\n");
                print_state();
                usleep(0.5 * 1000 * 1000);
        }
}

int main(int argc, char *argv[])
{
        if(argc < 2 || argc > 3) {
                printf("usage: ./a.out <num chairs> [customer sleep offset]\n\n");
                printf("  using customer sleep offset of -1 or -2 will make chairs fill\n");
                printf("  faster than the barber can complete haircuts. using 1 or 2 will\n");
                printf("  have the opposite effect.\n");
                exit(0);
        }

        num_chairs = atoi(argv[1]);
        printf("barber shop has %d chairs\n", num_chairs);

        customer_sleep_offset = argc >= 3 ? atoi(argv[2]) : 0;
        if(customer_sleep_offset != 0) {
                printf("using %d sec customer sleep offset\n", customer_sleep_offset);
        }

        // Initialize states and counters
        filled_chairs = 0;
        count_customers_serviced = 0;
        count_customers_rejected = 0;
        barber_state = BARBER_SLEEPING;

        // Initialize all one-off mutexes and conditions
        pthread_mutex_init(&print_lock, NULL);
        pthread_mutex_init(&count_lock, NULL);
        pthread_cond_init(&barber_wake_cond, NULL);

        // Initialize chair states, conditions, and locks
        chair_states = malloc(sizeof(int) * num_chairs);
        haircut_conds = malloc(sizeof(pthread_cond_t) * num_chairs);
        chair_locks = malloc(sizeof(pthread_mutex_t) * num_chairs);
        for(int i = 0; i < num_chairs; i++) {
                chair_states[i] = STATE_EMPTY;
                pthread_cond_init(&haircut_conds[i], NULL);
                pthread_mutex_init(&chair_locks[i], NULL);
        }

        // Start barber thread, continously start customer threads
        pthread_t t;
        pthread_create(&t, NULL, barber, NULL);
        pthread_create(&t, NULL, print_thread, NULL);
        while(1) {
                pthread_create(&t, NULL, customer, NULL);
                sleep(rand() % 4 + 2 + customer_sleep_offset);
        }
}
