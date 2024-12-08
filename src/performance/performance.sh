#!/bin/bash

TIMEFORMAT=%R

THREAD_CONFIGS=(2 4 8 16 32)
REPEATS=5
OUTPUT_FILE="src/performance/performance.csv"
EXECUTABLE_DIR="./target/bin"
PROGRAMS=("phil" "prod" "rw")

# Supprimer le fichier si il existe déjà
rm -f $OUTPUT_FILE

# Fichier CSV (sections)
echo "Programme,Nombre de Threads,Tentative N°,Temps d'execution" >$OUTPUT_FILE

make clean
make libc
# Tester chaque type 1 par 1
for PROGRAM in "${PROGRAMS[@]}"; do
  echo "Test du programme: $PROGRAM..."

  for THREADS in "${THREAD_CONFIGS[@]}"; do
    echo "  Test de $THREADS threads..."

    for RUN in $(seq 1 $REPEATS); do
      DURATION=""

      DURATION=$({ time $EXECUTABLE_DIR/$PROGRAM $THREADS 2>>/dev/null; } 2>&1)

      echo "$PROGRAM,$THREADS,$RUN,$DURATION" >>$OUTPUT_FILE
      echo "    Execution $RUN finie en $DURATION ms."
    done
  done
done

echo "All tests completed"
