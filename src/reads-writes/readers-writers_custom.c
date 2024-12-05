#include "my_mutex.h"
#include "pthread.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>


my_semaphore_t rmutex;
my_semaphore_t wmutex;
my_semaphore_t readTry;
my_semaphore_t resource;
int writers = 0;
int readers = 0;

void heavy_task() {
  for (int i = 0; i < 10000; i++)
    ;
}

void *reader() {
  for (int i = 0; i < 2540; i++) {

    sem_wait(&readTry);
    sem_wait(&rmutex);
    readers += 1;

    if (readers == 1) {
      sem_wait(&resource);
    }
    sem_post(&rmutex);
    sem_post(&readTry);

    // enter critical section

    // printf("Reader access\n");
    heavy_task();

    // end critical
    sem_wait(&rmutex);
    readers -= 1;
    if (readers == 0) {
      sem_post(&resource);
    }
    sem_post(&rmutex);
  }
  return NULL;
}

void *writer() {

  for (int i = 0; i < 640; i++) {
    sem_wait(&wmutex);
    writers += 1;
    if (writers == 1) {
      sem_wait(&readTry); // lock readers
    }

    sem_post(&wmutex);

    sem_wait(&resource);
    // enter critical

    // printf("Writer access\n");
    heavy_task();

    // end critical
    sem_post(&resource);

    sem_wait(&wmutex);
    writers -= 1;

    if (writers == 0) {
      sem_post(&readTry);
    }

    sem_post(&wmutex);
  }

  return NULL;
}

int run_readers_writers(int readers, int writers) {
  pthread_t readers_threads[readers];
  pthread_t writers_threads[writers];

  sem_init(&rmutex, 1);
  sem_init(&wmutex, 1);
  sem_init(&resource, 1);
  sem_init(&readTry, 1);

  for (int i = 0; i < readers; i++) {
    pthread_create(&readers_threads[i], NULL, reader, NULL);
  }

  for (int i = 0; i < writers; i++) {
    pthread_create(&writers_threads[i], NULL, writer, NULL);
  }

  for (int i = 0; i < readers; i++) {
    pthread_join(readers_threads[i], NULL);
  }

  for (int i = 0; i < writers; i++) {
    pthread_join(writers_threads[i], NULL);
  }

  sem_destroy(&wmutex);
  sem_destroy(&rmutex);
  sem_destroy(&resource);
  sem_destroy(&readTry);

  return 0;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Not enough arguments: READERS WRITERS\n");
    }

    char *raw_readers = argv[1];
    char *end_readers;
    int readers = strtol(raw_readers, &end_readers, 10);

    if (*end_readers != '\0') {
        printf("Failed to parse readers arguments\n");
    }

    char *raw_writers = argv[2];
    char *end_writers;
    int writers = strtol(raw_writers, &end_writers, 10);

    if (*end_writers != '\0') {
        printf("Failed to parse writers arguments\n");
    }

    return run_readers_writers(readers, writers);
}
