#include "readers-writers.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Not enough arguments: READERS WRITERS\n");
  }

  char *raw_readers = argv[1];
  char *end_readers;
  int readers = strtol(raw_readers, &end_readers, 10);

  if (*end_readers != '\0') {
    printf("Failed to parse readers arguments\n");
  }

  char *raw_writers = argv[2];
  char *end_writers;
  int writers = strtol(raw_writers, &end_writers, 10);

  if (*end_writers != '\0') {
    printf("Failed to parse writers arguments\n");
  }

  return run_readers_writers(readers, writers);
}
