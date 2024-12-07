#include "my_mutex.h"
#include <stdio.h>

void my_mutex_init(my_mutex_t *mutex) { mutex->flag = 0; }

void lock(my_mutex_t *mutex) {
  asm("trylock:"            // point d'entrée
      "movl $1, %%eax;"     // charge dans le registre eax la valeur 1
      "xchgl %%eax, %0;"    // Swap la valeur du mutex_test_and_set et du registre
      "testl %%eax, %%eax;" // test du state précédent du mutex_test_and_set
      "jnz trylock;"        // si il est à 1, il est tjrs bloqué, on recommence
      : "=m"(mutex->flag)
      : "m"(mutex->flag)
      : "eax");
}

void unlock(my_mutex_t *mutex) {
  asm("movl $0, %%eax;" // Charge "0" (unlocked) dans eax
      "xchgl %%eax, %0" // Swap eax et le mutex_test_and_set
      : "=m"(mutex->flag)
      : "m"(mutex->flag)
      : "eax");
}
