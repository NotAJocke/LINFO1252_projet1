#include "my_mutex.h"
#include "philosophers.h"
#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int id;
  my_mutex_t *forks;
  int philosophers_amount;
} phil_args_custom_t;

void *philosopher_custom(void *raw_args) {
  phil_args_custom_t *args = (phil_args_custom_t *)raw_args;

  int right = args->id;
  int left = (right + 1) % args->philosophers_amount;

  for (int i = 0; i < 1000000; i++) {
    if (right < left) {
      // printf("Phil %d taking the fork %d\n", right, right);
      lock(&args->forks[right]);

      // printf("Phil %d taking the fork %d\n", right, left);
      lock(&args->forks[left]);
    } else {
      // printf("Phil %d taking the fork %d\n", right, left);
      lock(&args->forks[left]);

      // printf("Phil %d taking the fork %d\n", right, right);
      lock(&args->forks[right]);
    }

    // printf("Phil %d releasing the fork %d\n", right, right);
    unlock(&args->forks[right]);

    // printf("Phil %d releasing the fork %d\n", right, left);
    unlock(&args->forks[left]);
  }

  pthread_exit(NULL);
}

int run_philosophers_custom(int amount) {
  pthread_t philosophers[amount];
  my_mutex_t forks[amount];
  phil_args_custom_t *args = malloc(amount * sizeof(phil_args_custom_t));
  if (args == NULL) {
    printf("Fatal: couldn't malloc the philosophers args\n");
    return 1;
  }

  // Init the mutexes
  for (int i = 0; i < amount; i++) {
    my_mutex_init(&forks[i]);
  }

  // Init the threads
  for (int i = 0; i < amount; i++) {
    args[i] = (phil_args_custom_t){
        .forks = forks, .philosophers_amount = amount, .id = i};

    pthread_create(&philosophers[i], NULL, philosopher_custom,
                   (void *)&args[i]);
  }

  for (int i = 0; i < amount; i++) {
    pthread_join(philosophers[i], NULL);
  }

  free(args);

  return 0;
}
