#include "philosophers.h"
#include <stdio.h>
#include <stdlib.h>

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
