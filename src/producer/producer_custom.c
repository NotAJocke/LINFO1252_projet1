#include "my_mutex.h"
#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE_CUSTOM 8
#define NB_PRODUCTIONS_CUSTOM 131072

int buffer_custom[BUFFER_SIZE_CUSTOM];
int idx_buffer_custom = 0;
int nbProductionsDone_custom = 0;
int nbConsumeDone_custom = 0;

my_mutex_t mutex_custom;
my_semaphore_t empty_custom;
my_semaphore_t full_custom;

// Simulation
void simulation_custom(void) {
  for (volatile int i = 0; i < 10000; i++)
    ;
}

void insert_item_custom(int item) {
  if (idx_buffer_custom >= BUFFER_SIZE_CUSTOM) {
    printf("Erreur : Segfault (Hors limite)\n");
    exit(EXIT_FAILURE);
  }
  buffer_custom[idx_buffer_custom] = item;
  idx_buffer_custom++;
}

int remove_item_custom() {
  if (idx_buffer_custom <= 0) {
    printf("Erreur : Segfault (Vide)\n");
    exit(EXIT_FAILURE);
  }
  idx_buffer_custom--;
  return buffer_custom[idx_buffer_custom];
}

void *producer_custom(void *arg) {
  int id = *(int *)arg;

  while (1) {
    int item = id;

    sem_wait(&empty_custom);
    lock(&mutex_custom);

    if (nbProductionsDone_custom >= NB_PRODUCTIONS_CUSTOM) {
      sem_post(&full_custom);
      unlock(&mutex_custom);
      break;
    }

    insert_item_custom(item);
    nbProductionsDone_custom++;

    sem_post(&full_custom);
    unlock(&mutex_custom);

    simulation_custom();
  }
  return NULL;
}

// Fonction consommateur
void *consumer_custom() {
  while (1) {
    sem_wait(&full_custom);
    lock(&mutex_custom);

    if (nbConsumeDone_custom >= NB_PRODUCTIONS_CUSTOM) {
      sem_post(&empty_custom);
      unlock(&mutex_custom);
      break;
    }

    remove_item_custom();
    nbConsumeDone_custom++;

    sem_post(&empty_custom);
    unlock(&mutex_custom);

    simulation_custom();
  }
  return NULL;
}

int run_producer_custom(int nbProducers, int nbConsumers) {
  if (nbProducers < 1 || nbConsumers < 1) {
    printf("Erreur : au moins 1 producteur et 1 consommateur sont requis.\n");
    return EXIT_FAILURE;
  }

  pthread_t producers[nbProducers];
  pthread_t consumers[nbConsumers];
  int producer_ids[nbProducers];

  my_mutex_init(&mutex_custom);
  sem_init(&empty_custom, BUFFER_SIZE_CUSTOM);
  sem_init(&full_custom, 0);

  // Création des threads producteurs
  for (int i = 0; i < nbProducers; i++) {
    producer_ids[i] = i + 1;
    pthread_create(&producers[i], NULL, producer_custom, &producer_ids[i]);
  }

  // Création des threads consommateurs
  for (int i = 0; i < nbConsumers; i++) {
    pthread_create(&consumers[i], NULL, consumer_custom, NULL);
  }

  // Attente de la fin des producteurs
  for (int i = 0; i < nbProducers; i++) {
    pthread_join(producers[i], NULL);
  }

  // Attente de la fin des consommateurs
  for (int i = 0; i < nbConsumers; i++) {
    pthread_join(consumers[i], NULL);
  }

  // Détruire les sémaphores
  // pthread_mutex_destroy(&mutex);
  sem_destroy(&empty_custom);
  sem_destroy(&full_custom);
  return EXIT_SUCCESS;
}
