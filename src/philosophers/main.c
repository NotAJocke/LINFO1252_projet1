#include "philosophers.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: ./phil <N_PHIL> <IMPL>\n");
    return 1;
  }

  char *raw_phils = argv[1];
  char *end;
  int amount = strtol(raw_phils, &end, 10);

  if (*end != '\0') {
    printf("Usage: ./phil <N_PHIL> <IMPL>\n");
    return 1;
  }

  char *raw_impl = argv[2];
  char *end2;
  int impl = strtol(raw_impl, &end2, 10);

  if (*end2 != '\0') {
    printf("Usage: ./phil <N_PHIL> <IMPL>\n");
    return 1;
  }

  if (impl == 1) {
    run_philosophers(amount);
  } else {
    run_philosophers_custom(amount);
  }

  return 0;
}
