#ifndef MY_MUTEX
#define MY_MUTEX

typedef struct {
  volatile int flag;
} my_mutex_t;

void my_mutex_init(my_mutex_t *mutex);
void lock(my_mutex_t *mutex);
void unlock(my_mutex_t *mutex);

typedef struct {
  volatile int count;
  my_mutex_t mutex;
} my_semaphore_t;

void sem_init(my_semaphore_t *sem, int initial_count);
void sem_wait(my_semaphore_t *sem);
void sem_post(my_semaphore_t *sem);
void sem_destroy(my_semaphore_t *sem);

#endif // !MY_MUTEX
