#!/bin/bash
TIMEFORMAT=%R

THREAD_CONFIGS=(2 4 8 16 32)
REPEATS=5
EXECUTABLE_DIR="./target/bin"
PROGRAMS=("mutex")

for MUTEX_IMPL in "tts" "ts"; do

  if [ "$MUTEX_IMPL" == "tts" ]; then
    OUTPUT_FILE="src/performance/performance_tts.csv"
  else
    OUTPUT_FILE="src/performance/performance_ts.csv"
  fi

  rm -f $OUTPUT_FILE
  echo "Programme,Nombre de Threads,Tentative N°,Temps d'execution" >$OUTPUT_FILE
  echo "Test du programme: $PROGRAM avec MUTEX_IMPL=$MUTEX_IMPL..."

  # suppr les fichier et reconstruire
  make clean
  make $MUTEX_IMPL

  # Tester chaque type 1 par 1 (il y en a qu'un pour l'instant)
  for PROGRAM in "${PROGRAMS[@]}"; do
    #Useless pour l'instant mais tkt
    for THREADS in "${THREAD_CONFIGS[@]}"; do
      echo "  Test de $THREADS threads avec MUTEX_IMPL=$MUTEX_IMPL..."

      for RUN in $(seq 1 $REPEATS); do
        DURATION=""
        DURATION=$({ time $EXECUTABLE_DIR/$PROGRAM $THREADS 2>>/dev/null; } 2>&1)

        echo "$PROGRAM,$THREADS,$RUN,$DURATION" >>$OUTPUT_FILE
        echo "    Execution $RUN finie en $DURATION ms."
      done
    done
  done
done

echo "All tests completed"
