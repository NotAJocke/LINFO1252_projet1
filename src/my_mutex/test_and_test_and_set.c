#include "my_mutex.h"
#include <stdio.h>

void my_mutex_init(my_mutex_t *mutex) { mutex->flag = 0; }

void lock(my_mutex_t *mutex) {
  asm("trylock:"            // point d'entrée
      " movl %0, %%eax;"    // Charge la valeur en mémoire du mutex dans eax
      "testl %%eax, %%eax;" // Test de la valeur du mutex
      "jz acquire_lock;"    // Si le mutex est à 0 (unlocked), on lock
      "jmp trylock;"     // sinon (donc quand il est à 1), on retourne au début
      "acquire_lock:"    // entrée de la tentative de lock
      "movl $1, %%eax;"  // charge "1" (locked) dans eax
      "xchgl %%eax, %0;" // Swap la valeur du mutex et eax
      "testl %%eax, %%eax;" // Check le state précédent
      "jnz trylock;"        // si il est à 1, il est tjrs bloqué, on recommence
      : "=m"(mutex->flag)
      : "m"(mutex->flag)
      : "eax");
}

void unlock(my_mutex_t *mutex) {
  asm("movl $0, %%eax;"  // Charge "0" (unlocked) dans eax
      "xchgl %%eax, %0;" // Swap eax et le mutex
      : "=m"(mutex->flag)
      : "m"(mutex->flag)
      : "eax");
}
