#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t agent_lock;
pthread_mutex_t pusher_lock;
pthread_mutex_t person_lock;
pthread_cond_t wake_agent;
pthread_cond_t tobacco, paper, matches;
pthread_cond_t wake_tobacco_person, wake_paper_person, wake_matches_person;
int isTobacco, isPaper, isMatches;

void *agent(void *ptr)
{
   while(1) {
      switch(random() % 3) {
         case 0:
            printf("[agent] placing down tobacco and papers\n");
            pthread_cond_signal(&tobacco);
            pthread_cond_signal(&paper);
            break;

         case 1:
            printf("[agent] placing down papers and matches\n");
            pthread_cond_signal(&paper);
            pthread_cond_signal(&matches);
            break;

         case 2:
            printf("[agent] placing down tobacco and matches\n");
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
      if(isPaper) {
         isPaper = 0;
         pthread_cond_signal(&wake_matches_person);
      } else if(isMatches) {
         isMatches = 0;
         pthread_cond_signal(&wake_paper_person);
      } else {
         isTobacco = 1;
      }
      pthread_mutex_unlock(&pusher_lock);
   }
}

void *pusherB(void *ptr)
{
   while(1) {
      pthread_mutex_lock(&pusher_lock);
      pthread_cond_wait(&paper, &pusher_lock);
      if(isMatches) {
         isMatches = 0;
         pthread_cond_signal(&wake_tobacco_person);
      } else if(isTobacco) {
         isTobacco = 0;
         pthread_cond_signal(&wake_matches_person);
      } else {
         isPaper = 1;
      }
      pthread_mutex_unlock(&pusher_lock);
   }
}

void *pusherC(void *ptr)
{
   while(1) {
      pthread_mutex_lock(&pusher_lock);
      pthread_cond_wait(&matches, &pusher_lock);
      if(isPaper) {
         isPaper = 0;
         pthread_cond_signal(&wake_tobacco_person);
      } else if(isTobacco) {
         isTobacco = 0;
         pthread_cond_signal(&wake_paper_person);
      } else {
         isMatches = 1;
      }
      pthread_mutex_unlock(&pusher_lock);
   }
}

void *tobacco_person(void *ptr)
{
   while(1) {
      pthread_cond_wait(&wake_tobacco_person, &person_lock);
      printf("[person] tobacco guy is smoking\n");
      pthread_mutex_unlock(&person_lock);

      sleep(random() % 3 + 1);
      pthread_cond_signal(&wake_agent);
   }
}

void *paper_person(void *ptr)
{
   while(1) {
      pthread_cond_wait(&wake_paper_person, &person_lock);
      printf("[person] paper guy is smoking\n");
      pthread_mutex_unlock(&person_lock);

      sleep(random() % 3 + 1);
      pthread_cond_signal(&wake_agent);
   }
}

void *matches_person(void *ptr)
{
   while(1) {
      pthread_cond_wait(&wake_matches_person, &person_lock);
      printf("[person] matches guy is smoking\n");
      pthread_mutex_unlock(&person_lock);

      sleep(random() % 3 + 1);
      pthread_cond_signal(&wake_agent);
   }
}

int main()
{
   isTobacco = isPaper = isMatches = 0;

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
