#include "platform_thread.h"
#include <stdlib.h>

//thread 

#ifdef _WIN32

typedef struct { void *(*fn)(void *); void *arg; } win_start_t;

static DWORD WINAPI win_trampoline(LPVOID p) {
    win_start_t *s = (win_start_t *)p;
    void *(*fn)(void *) = s->fn;
    void *arg = s->arg;
    free(s);
    fn(arg);
    return 0;
}

int pt_thread_create(pt_thread_t *t, void *(*fn)(void *), void *arg) {
    win_start_t *s = (win_start_t *)malloc(sizeof(*s));
    if (!s) return -1;
    s->fn = fn; s->arg = arg;
    t->handle = CreateThread(NULL, 0, win_trampoline, s, 0, NULL);
    if (t->handle == NULL) { free(s); return -1; }
    return 0;
}

int pt_thread_join(pt_thread_t *t, void **ret) {
    (void)ret;   
    WaitForSingleObject(t->handle, INFINITE);
    CloseHandle(t->handle);
    t->handle = NULL;
    return 0;
}

#else

int pt_thread_create(pt_thread_t *t, void *(*fn)(void *), void *arg) {
    return pthread_create(&t->handle, NULL, fn, arg);
}

int pt_thread_join(pt_thread_t *t, void **ret) {
    return pthread_join(t->handle, ret);
}

#endif

//mutex

#ifdef _WIN32

int pt_mutex_init(pt_mutex_t *m)    { InitializeCriticalSection(&m->cs); return 0; }
int pt_mutex_lock(pt_mutex_t *m)    { EnterCriticalSection(&m->cs); return 0; }
int pt_mutex_unlock(pt_mutex_t *m)  { LeaveCriticalSection(&m->cs); return 0; }
int pt_mutex_destroy(pt_mutex_t *m) { DeleteCriticalSection(&m->cs); return 0; }

#else

int pt_mutex_init(pt_mutex_t *m)    { return pthread_mutex_init(&m->mtx, NULL); }
int pt_mutex_lock(pt_mutex_t *m)    { return pthread_mutex_lock(&m->mtx); }
int pt_mutex_unlock(pt_mutex_t *m)  { return pthread_mutex_unlock(&m->mtx); }
int pt_mutex_destroy(pt_mutex_t *m) { return pthread_mutex_destroy(&m->mtx); }

#endif

//spin-lock

#ifdef _WIN32

#include <intrin.h>   //_mm_pause() 

int pt_spinlock_init(pt_spinlock_t *s) {
    s->flag = 0;
    return 0;
}

int pt_spinlock_lock(pt_spinlock_t *s) {
    while (InterlockedExchange(&s->flag, 1) != 0) {
        _mm_pause();   // reduz pressao no barramento e consumo de energia 
    }
    return 0;
}

int pt_spinlock_unlock(pt_spinlock_t *s) {
    InterlockedExchange(&s->flag, 0);
    return 0;
}

int pt_spinlock_destroy(pt_spinlock_t *s) {
    (void)s;
    return 0;
}

#else

int pt_spinlock_init(pt_spinlock_t *s) {
    s->flag = 0;
    return 0;
}

int pt_spinlock_lock(pt_spinlock_t *s) {
    while (__atomic_exchange_n(&s->flag, 1, __ATOMIC_ACQUIRE) != 0) {
        __builtin_ia32_pause();
    }
    return 0;
}

int pt_spinlock_unlock(pt_spinlock_t *s) {
    __atomic_store_n(&s->flag, 0, __ATOMIC_RELEASE);
    return 0;
}

int pt_spinlock_destroy(pt_spinlock_t *s) {
    (void)s;
    return 0;
}

#endif

//tempo

#ifdef _WIN32

uint64_t pt_now_ns(void) {
    static LARGE_INTEGER freq = {0};
    LARGE_INTEGER now;
    if (freq.QuadPart == 0) QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&now);
    return (uint64_t)((now.QuadPart * 1000000000ULL) / (uint64_t)freq.QuadPart);
}

#else

uint64_t pt_now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

#endif