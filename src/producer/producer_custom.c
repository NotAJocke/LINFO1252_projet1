#include "my_mutex.h"
#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 8
#define NB_PRODUCTIONS 131072

int buffer[BUFFER_SIZE];
int idx_buffer = 0;
int nbProductionsDone = 0;
int nbConsumeDone = 0;

my_mutex_t mutex;
my_semaphore_t empty;
my_semaphore_t full;

// Simulation
void simulation(void) {
    for (int i = 0; i < 10000; i++);
}

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

void *producer(void *arg) {
    int id = *(int *)arg;

    while (1) {
        int item = id;

        sem_wait(&empty);
        lock(&mutex);

        if (nbProductionsDone >= NB_PRODUCTIONS) {
            sem_post(&full);
            unlock(&mutex);
            break;
        }

        insert_item(item);
        nbProductionsDone++;

        sem_post(&full);
        unlock(&mutex);

        simulation();
    }
    return NULL;
}

// Fonction consommateur
void *consumer() {
    while (1) {
        sem_wait(&full);
        lock(&mutex);

        if (nbConsumeDone >= NB_PRODUCTIONS) {
            sem_post(&empty);
            unlock(&mutex);
            break;
        }

        remove_item();
        nbConsumeDone++;

        sem_post(&empty);
        unlock(&mutex);

        simulation();
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <num_producers> <num_consumers>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int nbProducers = atoi(argv[1]);
    int nbConsumers = atoi(argv[2]);

    if (nbProducers < 1 || nbConsumers < 1) {
        printf("Erreur : au moins 1 producteur et 1 consommateur sont requis.\n");
        return EXIT_FAILURE;
    }

    pthread_t producers[nbProducers];
    pthread_t consumers[nbConsumers];
    int producer_ids[nbProducers];

    my_mutex_init(&mutex);
    sem_init(&empty, BUFFER_SIZE);
    sem_init(&full, 0);

    // Création des threads producteurs
    for (int i = 0; i < nbProducers; i++) {
        producer_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }

    // Création des threads consommateurs
    for (int i = 0; i < nbConsumers; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
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
    sem_destroy(&empty);
    sem_destroy(&full);
    return EXIT_SUCCESS;
}
