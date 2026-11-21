/* produtor/consumidor com semáforos POSIX */
#include <semaphore.h>
#define N 100  
sem_t vazio, cheio, mutex;
int buf[N], in = 0, out = 0;
                     /* number of slots in the buffer */

void consumer(void)
{
    int item;

    while (TRUE) {                  /* infinite loop */
        down(&full);                /* decrement full count */
        down(&mutex);               /* enter critical region */
        item = remove_item();       /* take item from buffer */
        up(&mutex);                 /* leave critical region */
        up(&empty);                 /* increment count of empty slots */
        consume_item(item);         /* do something with the item */
    }
}

void *produtor(void *a) {
    for (;;) {
        int item = produz();
        sem_wait(&vazio); /* down(empty) */
        sem_wait(&mutex); /* down(mutex) */
        buf[in] = item; in = (in + 1) % N;
        sem_post(&mutex); /* up(mutex) */
        sem_post(&cheio); /* up(full) */
    }
}
/* main: sem_init(&vazio,0,N); sem_init(&cheio,0,0);
 s em_init(&mutex,0,1); compile com -pthread */*
