#!/bin/bash

THREAD_CONFIGS=(2 4 8 16 32)
REPEATS=5
OUTPUT_FILE="performance.csv"
EXECUTABLE_DIR="./target/bin"
PROGRAMS=("phil" "pro" "rw")

# Supprimer le fichier si il existe déjà
rm -f $OUTPUT_FILE

# Fichier CSV (sections)
echo "Programme,Nombre de Threads,Tentative N°,Temps d'execution" > $OUTPUT_FILE

# Tester chaque type 1 par 1
for PROGRAM in "${PROGRAMS[@]}"; do
    echo "Test du programme: $PROGRAM..."

    for THREADS in "${THREAD_CONFIGS[@]}"; do
        echo "  Test de $THREADS threads..."

        # Diviser les threads en deux pour produce et reader-writers
        NB_PRODUCERS=$((THREADS / 2))
        NB_CONSUMERS=$((THREADS / 2))

        for RUN in $(seq 1 $REPEATS); do
            START_TIME=$(date +"%s%3N")

            if [[ "$PROGRAM" == "phil" ]]; then
                $EXECUTABLE_DIR/$PROGRAM $THREADS > /dev/null 2>&1
            else
                $EXECUTABLE_DIR/$PROGRAM $NB_PRODUCERS $NB_CONSUMERS > /dev/null 2>&1
            fi

            END_TIME=$(date +"%s%3N")
            DURATION=$(echo "$END_TIME - $START_TIME" | bc)
            DURATION_SEC=$(echo "scale=3; $DURATION / 1000" | bc)

            echo "$PROGRAM,$THREADS,$RUN,$DURATION_SEC" >> $OUTPUT_FILE
            echo "    Execution $RUN finie en $DURATION_SEC secondes."
        done
    done
done

echo "All tests completed"
