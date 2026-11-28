#ifndef PLATFORM_THREAD_H
#define PLATFORM_THREAD_H

#include <stdint.h>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <pthread.h>
#endif
//thread

typedef struct pt_thread {
#ifdef _WIN32
    HANDLE handle;
#else
    pthread_t handle;
#endif
} pt_thread_t;

int pt_thread_create(pt_thread_t *t, void *(*fn)(void *), void *arg);
int pt_thread_join(pt_thread_t *t, void **ret);

//mutex
typedef struct pt_mutex {
#ifdef _WIN32
    CRITICAL_SECTION cs;
#else
    pthread_mutex_t mtx;
#endif
} pt_mutex_t;

int pt_mutex_init(pt_mutex_t *m);
int pt_mutex_lock(pt_mutex_t *m);
int pt_mutex_unlock(pt_mutex_t *m);
int pt_mutex_destroy(pt_mutex_t *m);

//spin-lock
typedef struct pt_spinlock {
#ifdef _WIN32
    volatile LONG flag;   // 0 = livre, 1 = ocupado 
#else
    volatile int flag;    // 0 = livre, 1 = ocupado 
#endif
} pt_spinlock_t;

int pt_spinlock_init(pt_spinlock_t *s);
int pt_spinlock_lock(pt_spinlock_t *s);
int pt_spinlock_unlock(pt_spinlock_t *s);
int pt_spinlock_destroy(pt_spinlock_t *s);

//tempo
uint64_t pt_now_ns(void);

#endif