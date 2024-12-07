#include "my_mutex.h"
#include "pthread.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

my_semaphore_t rmutex;
my_semaphore_t wmutex;
my_semaphore_t readTry;
my_semaphore_t resource;
int writers = 0;
int readers = 0;

void heavy_task() {
  for (volatile int i = 0; i < 10000; i++)
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
  printf("Running the custom one\n");

  if (argc < 2) {
    printf("Usage: ./phil <READERS+WRITERS>\n");
  }

  char *raw = argv[1];
  char *end;
  int amount = strtol(raw, &end, 10);

  if (*end != '\0') {
    printf("Usage: ./phil <READERS+WRITERS>\n");
  }

  int readers = amount / 2;
  int writers = amount / 2;
  run_readers_writers(readers, writers);
}
