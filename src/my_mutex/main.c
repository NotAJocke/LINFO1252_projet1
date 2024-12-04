#include "my_mutex.h"
#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>

#define CRITICAL_SECTIONS 32768

my_mutex_t mutex;

void *fake_worker(void *arg) {
  (void)arg;

  for (int i = 0; i < CRITICAL_SECTIONS; i++) {
    lock(&mutex);

    for (int i = 0; i < 10000; i++)
      ;

    unlock(&mutex);
  }

  return NULL;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: ./mutex <NTHREADS>\n");
    return 1;
  }

  char *end;
  int nthreads = strtol(argv[1], &end, 10);

  if (*end != '\0') {
    printf("Usage: ./mutex <NTHREADS>\n");
    return 1;
  }

  my_mutex_init(&mutex);

  pthread_t threads[nthreads];

  for (int i = 0; i < nthreads; i++) {
    pthread_create(&threads[i], NULL, fake_worker, NULL);
  }

  for (int i = 0; i < nthreads; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}
