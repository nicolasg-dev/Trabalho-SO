#include "platform_thread.h"
#include <stdio.h>

#define ITER 100000
#define N 4

static long contador;
static pt_spinlock_t spin;

static void *worker(void *arg) {
    (void)arg;
    for (int i = 0; i < ITER; i++) {
        pt_spinlock_lock(&spin);
        contador++;
        pt_spinlock_unlock(&spin);
    }
    return NULL;
}

int main(void) {
    contador = 0;
    pt_spinlock_init(&spin);

    pt_thread_t t[N];
    for (int i = 0; i < N; i++) pt_thread_create(&t[i], worker, NULL);
    for (int i = 0; i < N; i++) pt_thread_join(&t[i], NULL);

    printf("contador = %ld (esperado %d)\n", contador, ITER * N);
    pt_spinlock_destroy(&spin);
    return 0;
}