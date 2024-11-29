#include "readers-writers.h"
#include "pthread.h"
// #include "semaphore.h"
#include "portable_semaphore.h"
#include <stdio.h>

int run_readers_writers(int readers, int writers) {
  printf("Readers: %d\n", readers);
  printf("Writers: %d\n", writers);

  sem_t *sem = sem_init(5);

  sem_wait(sem);

  sem_wait(sem);

  sem_wait(sem);

  sem_destroy(sem);

  return 0;
}
