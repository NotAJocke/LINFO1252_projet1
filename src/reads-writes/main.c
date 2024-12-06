#include "readers-writers.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 4) {
    printf("Usage: ./phil <READERS> <WRITERS> <IMPl>\n");
  }

  char *raw_readers = argv[1];
  char *end_readers;
  int readers = strtol(raw_readers, &end_readers, 10);

  if (*end_readers != '\0') {
    printf("Usage: ./phil <READERS> <WRITERS> <IMPl>\n");
  }

  char *raw_writers = argv[2];
  char *end_writers;
  int writers = strtol(raw_writers, &end_writers, 10);

  if (*end_writers != '\0') {
    printf("Usage: ./phil <READERS> <WRITERS> <IMPl>\n");
  }

  char *raw_impl = argv[3];
  char *end_impl;
  int impl = strtol(raw_impl, &end_impl, 10);

  if (*end_impl != '\0') {
    printf("Usage: ./phil <READERS> <WRITERS> <IMPl>\n");
  }

  if (impl == 1) {
    run_readers_writers(readers, writers);
  } else {
    run_readers_writers_custom(readers, writers);
  }
}
