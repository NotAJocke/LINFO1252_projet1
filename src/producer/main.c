#include "producer.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: ./prod <PRODUCERS> <CONSUMERS> <IMPL>\n");
    return 1;
  }

  char *raw_prod = argv[1];
  char *end;
  int prods = strtol(raw_prod, &end, 10);

  if (*end != '\0') {
    printf("Usage: ./prod <PRODUCERS> <CONSUMERS> <IMPL>\n");
    return 1;
  }

  char *raw_consum = argv[2];
  char *end2;
  int consum = strtol(raw_consum, &end2, 10);

  if (*end2 != '\0') {
    printf("Usage: ./prod <PRODUCERS> <CONSUMERS> <IMPL>\n");
    return 1;
  }

  char *raw_impl = argv[3];
  char *end3;
  int impl = strtol(raw_impl, &end3, 10);

  if (*end3 != '\0') {
    printf("Usage: ./prod <PRODUCERS> <CONSUMERS> <IMPL>\n");
    return 1;
  }

  if (impl == 1) {
    run_producer(prods, consum);
  } else {
    run_producer_custom(prods, consum);
  }

  return 0;
}
