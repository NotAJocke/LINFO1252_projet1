#include "my_mutex.h"
#include <stdio.h>

void sem_init(my_semaphore_t *sem, int initial_count) {
  sem->count = initial_count;
  my_mutex_init(&sem->mutex);
}

void sem_wait(my_semaphore_t *sem) {
  lock(&sem->mutex);

  while (sem->count <= 0) {
    unlock(&sem->mutex);
    printf("Waiting, current count: %d\n", sem->count);
    asm("pause");
    lock(&sem->mutex);
  }

  sem->count -= 1;

  unlock(&sem->mutex);
}

void sem_post(my_semaphore_t *sem) {
  lock(&sem->mutex);

  sem->count += 1;

  unlock(&sem->mutex);
}

void sem_destroy(my_semaphore_t *sem) { unlock(&sem->mutex); }
