#include "my_mutex.h"
#include "pthread.h"
#include <stdio.h>

// int main() {
//   my_mutex_t mutex;
//
//   my_mutex_init(&mutex);
//
//   printf("Default state: %d\n", mutex.flag);
//
//   lock(&mutex);
//
//   printf("State once locked: %d\n", mutex.flag);
//
//   unlock(&mutex);
//
//   printf("State once unlocked: %d\n", mutex.flag);
// }

#define NTHREADS 4
#define INCREMENTS_PER_THREAD 100000

int counter = 0;
my_mutex_t mutex;

void *incrementer(void *arg) {
  (void)arg;

  for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
    lock(&mutex);
    counter += 1;
    unlock(&mutex);
  }

  return NULL;
}

int main() {
  pthread_t threads[NTHREADS];

  my_mutex_init(&mutex);

  for (int i = 0; i < NTHREADS; i++) {
    pthread_create(&threads[i], NULL, incrementer, NULL);
  }

  for (int i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Expected counter value: %d\n", NTHREADS * INCREMENTS_PER_THREAD);
  printf("Actual counter value: %d\n", counter);

  return 0;
}
