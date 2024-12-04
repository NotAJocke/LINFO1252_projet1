#ifndef MY_MUTEX
#define MY_MUTEX

typedef struct {
  volatile int flag;
} my_mutex_t;

void my_mutex_init(my_mutex_t *mutex);
void lock(my_mutex_t *mutex);
void unlock(my_mutex_t *mutex);

#endif // !MY_MUTEX
