#include "my_mutex.h"
#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int id;
  my_mutex_t *forks;
  int philosophers_amount;
} phil_args_t;

void *philosopher(void *raw_args) {
  phil_args_t *args = (phil_args_t *)raw_args;

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

int run_philosophers(int amount) {
  pthread_t philosophers[amount];
  my_mutex_t forks[amount];
  phil_args_t *args = malloc(amount * sizeof(phil_args_t));
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
    args[i] =
        (phil_args_t){.forks = forks, .philosophers_amount = amount, .id = i};

    pthread_create(&philosophers[i], NULL, philosopher, (void *)&args[i]);
  }

  for (int i = 0; i < amount; i++) {
    pthread_join(philosophers[i], NULL);
  }

  free(args);

  return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Missing number argument\n");
        return 1;
    }

    char *raw_number = argv[1];
    char *end;
    int amount = strtol(raw_number, &end, 10);

    if (*end != '\0') {
        printf("Failed to parse number argument\n");
        return 1;
    }

    run_philosophers(amount);
    return 0;
}
