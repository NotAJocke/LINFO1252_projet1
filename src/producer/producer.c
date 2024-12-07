#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 8
#define NB_PRODUCTIONS 131072

int buffer[BUFFER_SIZE];
int idx_buffer = 0;
int nbProductionsDone = 0;
int nbConsumeDone = 0;

pthread_mutex_t mutex;
sem_t empty;
sem_t full;
// Simulation
void simulation(void) {
  for (volatile int i = 0; i < 10000; i++)
    ;
}

// Insertion d'un élément dans le buffer -> Ma seg error est ici
// (probablement que juste faire idx_buffer >= BUFFER_SIZE ne convient pas, mais
// jsp quoi essayer d'autre)
void insert_item(int item) {
  if (idx_buffer >= BUFFER_SIZE) {
    printf("Erreur : Segfault (Hors limite)\n");
    exit(EXIT_FAILURE);
  }
  buffer[idx_buffer] = item;
  idx_buffer++;
}

int remove_item() {
  if (idx_buffer <= 0) {
    printf("Erreur : Segfault (Vide)\n");
    exit(EXIT_FAILURE);
  }
  idx_buffer--;
  return buffer[idx_buffer];
}

// Fonction producteur
void *producer(void *arg) {
  int id = *(int *)arg;

  while (1) {
    printf("entering producer\n");
    int item = id;

    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    if (nbProductionsDone >= NB_PRODUCTIONS) {
      sem_post(&full);
      pthread_mutex_unlock(&mutex);
      break;
    }

    insert_item(item);
    nbProductionsDone++;

    sem_post(&full);
    pthread_mutex_unlock(&mutex);

    simulation();
  }
  return NULL;
}

// Fonction consommateur
void *consumer() {
  while (1) {
    printf("entering consummer\n");
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    if (nbConsumeDone >= NB_PRODUCTIONS) {
      sem_post(&empty);
      pthread_mutex_unlock(&mutex);
      break;
    }

    remove_item();
    nbConsumeDone++;

    sem_post(&empty);
    pthread_mutex_unlock(&mutex);

    simulation();
  }
  return NULL;
}

int run_producer(int nbProducers, int nbConsumers) {
  if (nbProducers < 1 || nbConsumers < 1) {
    printf("Erreur : au moins 1 producteur et 1 consommateur sont requis.\n");
    return EXIT_FAILURE;
  }

  pthread_t producers[nbProducers];
  pthread_t consumers[nbConsumers];
  int producer_ids[nbProducers];

  pthread_mutex_init(&mutex, NULL);
  sem_init(&empty, 0, BUFFER_SIZE);
  sem_init(&full, 0, 0);

  for (int i = 0; i < nbProducers; i++) {
    producer_ids[i] = i + 1;
    pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
  }

  for (int i = 0; i < nbConsumers; i++) {
    pthread_create(&consumers[i], NULL, consumer, NULL);
  }

  for (int i = 0; i < nbProducers; i++) {
    pthread_join(producers[i], NULL);
  }

  for (int i = 0; i < nbConsumers; i++) {
    pthread_join(consumers[i], NULL);
  }

  pthread_mutex_destroy(&mutex);
  sem_destroy(&empty);
  sem_destroy(&full);

  return EXIT_SUCCESS;
}
