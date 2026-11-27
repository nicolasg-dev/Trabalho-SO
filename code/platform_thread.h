#ifdef _WIN32
#include <windows.h>
//#include <synchapi.h>
//#include <processthreadsapi.h>

    typedef CRITICAL_SECTION pt_mutex_t;
    typedef HANDLE pt_t;
#else
#include <pthread.h>
    typedef unsigned long DWORD;
    typedef pthread_mutex_t pt_mutex_t;
    typedef void* LPVOID;
    typedef pthread_t pt_t;
#endif

int benchmark (char *argv[], int qual);
int pt_mutex_init(pt_mutex_t *mutex);
int pt_mutex_lock(pt_mutex_t *mutex);
int pt_mutex_trylock(pt_mutex_t *mutex);
int pt_mutex_unlock(pt_mutex_t *mutex);
int pt_mutex_destroy(pt_mutex_t *mutex);
int pt_join(pt_t thread);
int pt_create(pt_t thread, 
    DWORD (*start_routine)(void *), void *arg);