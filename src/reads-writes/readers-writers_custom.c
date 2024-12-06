#include "my_mutex.h"
#include "pthread.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

my_semaphore_t rmutex_custom;
my_semaphore_t wmutex_custom;
my_semaphore_t readTry_custom;
my_semaphore_t resource_custom;
int writers_custom = 0;
int readers_custom = 0;

void heavy_task_custom() {
  for (volatile int i = 0; i < 10000; i++)
    ;
}

void *reader_custom() {
  for (int i = 0; i < 2540; i++) {

    sem_wait(&readTry_custom);
    sem_wait(&rmutex_custom);
    readers_custom += 1;

    if (readers_custom == 1) {
      sem_wait(&resource_custom);
    }
    sem_post(&rmutex_custom);
    sem_post(&readTry_custom);

    // enter critical section

    // printf("Reader access\n");
    heavy_task_custom();

    // end critical
    sem_wait(&rmutex_custom);
    readers_custom -= 1;
    if (readers_custom == 0) {
      sem_post(&resource_custom);
    }
    sem_post(&rmutex_custom);
  }
  return NULL;
}

void *writer_custom() {

  for (int i = 0; i < 640; i++) {
    sem_wait(&wmutex_custom);
    writers_custom += 1;
    if (writers_custom == 1) {
      sem_wait(&readTry_custom); // lock readers
    }

    sem_post(&wmutex_custom);

    sem_wait(&resource_custom);
    // enter critical

    // printf("Writer access\n");
    heavy_task_custom();

    // end critical
    sem_post(&resource_custom);

    sem_wait(&wmutex_custom);
    writers_custom -= 1;

    if (writers_custom == 0) {
      sem_post(&readTry_custom);
    }

    sem_post(&wmutex_custom);
  }

  return NULL;
}

int run_readers_writers_custom(int readers, int writers) {
  pthread_t readers_threads[readers];
  pthread_t writers_threads[writers];

  sem_init(&rmutex_custom, 1);
  sem_init(&wmutex_custom, 1);
  sem_init(&resource_custom, 1);
  sem_init(&readTry_custom, 1);

  for (int i = 0; i < readers; i++) {
    pthread_create(&readers_threads[i], NULL, reader_custom, NULL);
  }

  for (int i = 0; i < writers; i++) {
    pthread_create(&writers_threads[i], NULL, writer_custom, NULL);
  }

  for (int i = 0; i < readers; i++) {
    pthread_join(readers_threads[i], NULL);
  }

  for (int i = 0; i < writers; i++) {
    pthread_join(writers_threads[i], NULL);
  }

  sem_destroy(&wmutex_custom);
  sem_destroy(&rmutex_custom);
  sem_destroy(&resource_custom);
  sem_destroy(&readTry_custom);

  return 0;
}
