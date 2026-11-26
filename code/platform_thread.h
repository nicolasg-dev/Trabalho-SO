#ifdef _WIN32
#include <windows.h>
//#include <synchapi.h>
//#include <processthreadsapi.h>

    typedef CRITICAL_SECTION pt_mutex_t;
    typedef HANDLE pthread_t;
#else
#include <pthread.h>
    typedef unsigned long int DWORD;
    typedef pthread_mutex_t pt_mutex_t;
    typedef void* LPVOID;
#endif

DWORD runner(LPVOID param);
int pt_mutex_init(pt_mutex_t *mutex);
int pt_mutex_lock(pt_mutex_t *mutex);
int pt_mutex_trylock(pt_mutex_t *mutex);
int pt_mutex_unlock(pt_mutex_t *mutex);
int pt_mutex_destroy(pt_mutex_t *mutex);
int pt_join(pthread_t thread, void **retval);
int pt_create(LPDWORD *tid
        , void* attr
        , LPTHREAD_START_ROUTINE function
        , LPVOID param);
