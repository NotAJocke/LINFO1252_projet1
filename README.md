# LINFO1252 - Project 1

## Compilation
```sh
$ make # Compile avec le mutex test_and_test_and_set
$ make MUTEX_IMPL="tts" # Pareil 
$ make MUTEX_IMPL="ts" # Avec le mutex test_and_set
```

## Run
```sh
$ make run BIN=<"phil"|"rw"|"prod"|"mutex"> ARGS="xxxxx"
$ ./target/bin/<phil|rw|prod|mutex> [args] # sinon
```
