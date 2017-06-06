#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

pthread_mutex_t agent_lock;
pthread_mutex_t pusher_lock;
pthread_mutex_t person_lock;
pthread_cond_t wake_agent;
pthread_cond_t tobacco, paper, matches;
pthread_cond_t wake_tobacco_person, wake_paper_person, wake_matches_person;
int is_tobacco, is_paper, is_matches;

char table_contents[30];
int thread_states[3];

void print_state()
{
    printf("\n\n\n\n");
    printf("on table: %s\n", table_contents);
    printf("person 1 (tobacco): %s\n", thread_states[0] ? "smoking" : "waiting");
    printf("person 2 (paper):   %s\n", thread_states[1] ? "smoking" : "waiting");
    printf("person 3 (matches): %s\n", thread_states[2] ? "smoking" : "waiting");
}

void *agent(void *ptr)
{
   while(1) {
      switch(random() % 3) {
         case 0:
            strcpy(table_contents, "tobacco and paper");
            pthread_cond_signal(&tobacco);
            pthread_cond_signal(&paper);
            break;

         case 1:
            strcpy(table_contents, "paper and matches");
            pthread_cond_signal(&paper);
            pthread_cond_signal(&matches);
            break;

         case 2:
            strcpy(table_contents, "tobacco and matches");
            pthread_cond_signal(&tobacco);
            pthread_cond_signal(&matches);
            break;
      }
      pthread_mutex_lock(&agent_lock);
      pthread_cond_wait(&wake_agent, &agent_lock);
      pthread_mutex_unlock(&agent_lock);
   }
}

void *pusherA(void *ptr)
{
   while(1) {
      pthread_mutex_lock(&pusher_lock);
      pthread_cond_wait(&tobacco, &pusher_lock);
      if(is_paper) {
         is_paper = 0;
         pthread_cond_signal(&wake_matches_person);
      } else if(is_matches) {
         is_matches = 0;
         pthread_cond_signal(&wake_paper_person);
      } else {
         is_tobacco = 1;
      }
      pthread_mutex_unlock(&pusher_lock);
   }
}

void *pusherB(void *ptr)
{
   while(1) {
      pthread_mutex_lock(&pusher_lock);
      pthread_cond_wait(&paper, &pusher_lock);
      if(is_matches) {
         is_matches = 0;
         pthread_cond_signal(&wake_tobacco_person);
      } else if(is_tobacco) {
         is_tobacco = 0;
         pthread_cond_signal(&wake_matches_person);
      } else {
         is_paper = 1;
      }
      pthread_mutex_unlock(&pusher_lock);
   }
}

void *pusherC(void *ptr)
{
   while(1) {
      pthread_mutex_lock(&pusher_lock);
      pthread_cond_wait(&matches, &pusher_lock);
      if(is_paper) {
         is_paper = 0;
         pthread_cond_signal(&wake_tobacco_person);
      } else if(is_tobacco) {
         is_tobacco = 0;
         pthread_cond_signal(&wake_paper_person);
      } else {
         is_matches = 1;
      }
      pthread_mutex_unlock(&pusher_lock);
   }
}

void *tobacco_person(void *ptr)
{
   while(1) {
      pthread_cond_wait(&wake_tobacco_person, &person_lock);
      pthread_mutex_unlock(&person_lock);

      thread_states[0] = 1;
      print_state();
      sleep(random() % 3 + 1);
      thread_states[0] = 0;

      pthread_cond_signal(&wake_agent);
   }
}

void *paper_person(void *ptr)
{
   while(1) {
      pthread_cond_wait(&wake_paper_person, &person_lock);
      pthread_mutex_unlock(&person_lock);

      thread_states[1] = 1;
      print_state();
      sleep(random() % 3 + 1);
      thread_states[1] = 0;

      pthread_cond_signal(&wake_agent);
   }
}

void *matches_person(void *ptr)
{
   while(1) {
      pthread_cond_wait(&wake_matches_person, &person_lock);
      pthread_mutex_unlock(&person_lock);

      thread_states[2] = 1;
      print_state();
      sleep(random() % 3 + 1);
      thread_states[2] = 0;

      pthread_cond_signal(&wake_agent);
   }
}

int main()
{
   is_tobacco = is_paper = is_matches = 0;

   pthread_mutex_init(&agent_lock, NULL);
   pthread_mutex_init(&pusher_lock, NULL);
   pthread_mutex_init(&person_lock, NULL);

   pthread_cond_init(&tobacco, NULL);
   pthread_cond_init(&paper, NULL);
   pthread_cond_init(&matches, NULL);

   pthread_cond_init(&wake_agent, NULL);

   pthread_cond_init(&wake_tobacco_person, NULL);
   pthread_cond_init(&wake_paper_person, NULL);
   pthread_cond_init(&wake_matches_person, NULL);

   pthread_t t;

   pthread_create(&t, NULL, tobacco_person, NULL);
   pthread_create(&t, NULL, paper_person, NULL);
   pthread_create(&t, NULL, matches_person, NULL);

   pthread_create(&t, NULL, pusherA, NULL);
   pthread_create(&t, NULL, pusherB, NULL);
   pthread_create(&t, NULL, pusherC, NULL);

   pthread_create(&t, NULL, agent, NULL);

   pthread_join(t, NULL);
}
