#ifdef _WIN32
#include <windows.h>
    typedef CRITICAL_SECTION pt_mutex_t;

#else
#include <pthread.h>
    typedef pthread_mutex_t pt_mutex_t;

#endif

int pt_mutex_init(pt_mutex_t *mutex);
int pt_mutex_lock(pt_mutex_t *mutex);
int pt_mutex_trylock(pt_mutex_t *mutex);
int pt_mutex_unlock(pt_mutex_t *mutex);
int pt_mutex_destroy(pt_mutex_t *mutex);
//int pt_join();