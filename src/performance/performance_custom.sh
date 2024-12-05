#!/bin/bash
TIMEFORMAT=%R

THREAD_CONFIGS=(2 4 8 16 32)
REPEATS=5
EXECUTABLE_DIR="./target/bin"
PROGRAMS=("phil_custom" "prod_custom" "rw_custom")

for MUTEX_IMPL in "tts" "ts"; do

  if [ "$MUTEX_IMPL" == "tts" ]; then
    OUTPUT_FILE="src/performance/performance_tts_custom.csv"
  else
    OUTPUT_FILE="src/performance/performance_ts_custom.csv"
  fi

  rm -f $OUTPUT_FILE
  echo "Programme,Nombre de Threads,Tentative N°,Temps d'execution" >$OUTPUT_FILE
  echo "Test du programme: $PROGRAM avec MUTEX_IMPL=$MUTEX_IMPL"

  # suppr les fichier et reconstruire
  make clean
  make MUTEX_IMPL=$MUTEX_IMPL

  # Tester chaque type 1 par 1 (il y en a qu'un pour l'instant)
  for PROGRAM in "${PROGRAMS[@]}"; do
    #Useless pour l'instant mais tkt
    for THREADS in "${THREAD_CONFIGS[@]}"; do
      echo "  Test de $THREADS threads avec (MUTEX : $MUTEX_IMPL & PROGRAM : $PROGRAM)"

      NB_PRODUCERS=$((THREADS / 2))
      NB_CONSUMERS=$((THREADS / 2))

      for RUN in $(seq 1 $REPEATS); do
        DURATION=""

        if [[ "$PROGRAM" == "phil_custom" ]]; then
          DURATION=$({ time $EXECUTABLE_DIR/$PROGRAM $THREADS 2>>/dev/null; } 2>&1)
        else
          DURATION=$({ time $EXECUTABLE_DIR/$PROGRAM $NB_PRODUCERS $NB_CONSUMERS 2>>/dev/null; } 2>&1)
        fi

        echo "$PROGRAM,$THREADS,$RUN,$DURATION" >>$OUTPUT_FILE
        echo "    Execution $RUN finie en $DURATION ms."
      done
    done
  done
done

echo "All tests completed"
