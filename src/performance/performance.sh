#!/bin/bash

TIMEFORMAT=%R

THREAD_CONFIGS=(2 4 8 16 32)
REPEATS=5
OUTPUT_FILE="src/performance/performance.csv"
EXECUTABLE_DIR="./target/bin"
PROGRAMS=("phil" "pro" "rw")

# Supprimer le fichier si il existe déjà
rm -f $OUTPUT_FILE

# Fichier CSV (sections)
echo "Programme,Nombre de Threads,Tentative N°,Temps d'execution" >$OUTPUT_FILE

# Tester chaque type 1 par 1
for PROGRAM in "${PROGRAMS[@]}"; do
  echo "Test du programme: $PROGRAM..."

  for THREADS in "${THREAD_CONFIGS[@]}"; do
    echo "  Test de $THREADS threads..."

    # Diviser les threads en deux pour produce et reader-writers
    NB_PRODUCERS=$((THREADS / 2))
    NB_CONSUMERS=$((THREADS / 2))

    for RUN in $(seq 1 $REPEATS); do
      DURATION=""

      if [[ "$PROGRAM" == "phil" ]]; then
        DURATION=$({ time make run BIN="$PROGRAM" ARGS="$THREADS 1"  2>>/dev/null; } 2>&1)
      else
          DURATION=$({ time make run BIN="$PROGRAM" ARGS="$NB_PRODUCERS $NB_CONSUMERS 1" 2>>/dev/null; } 2>&1)
      fi

      echo "$PROGRAM,$THREADS,$RUN,$DURATION" >>$OUTPUT_FILE
      echo "    Execution $RUN finie en $DURATION ms."
    done
  done
done

echo "All tests completed"
