#include <windows.h>
#include <pthread.h>

#ifdef _WIN32

    typedef CRITICAL_SECTION pt_mutex_t;



    int pt_mutex_init(pt_mutex_t *mutex,
                           const pt_mutexattr_t *mutexattr);
    int pt_mutex_lock(pt_mutex_t *mutex);
    int pt_mutex_trylock(pt_mutex_t *mutex);
    int pt_mutex_unlock(pt_mutex_t *mutex);
    int pt_mutex_destroy(pt_mutex_t *mutex);


    TryEnterCriticalSection(&cs)

        // 1. Initialize once (e.g., at startup)
    InitializeCriticalSectionAndSpinCount(&cs, 4000);

    // 2. Lock before accessing shared data
    EnterCriticalSection(&cs);
    // ... critical code / shared resource access ...
    
    // 3. Unlock when done
    LeaveCriticalSection(&cs);

    // 4. Clean up (e.g., at shutdown)
    DeleteCriticalSection(&cs);

#else
    typedef pthread_mutex_t pt_mutex_t;


    pt_mutex_t fastmutex = PT_MUTEX_INITIALIZER;
    pt_mutex_t recmutex = PT_RECURSIVE_MUTEX_INITIALIZER_NP;
    pt_mutex_t errchkmutex = PT_ERRORCHECK_MUTEX_INITIALIZER_NP;

    int pt_mutex_init(pt_mutex_t *mutex,
                           const pt_mutexattr_t *mutexattr);
    int pt_mutex_lock(pt_mutex_t *mutex);
    int pt_mutex_trylock(pt_mutex_t *mutex);
    int pt_mutex_unlock(pt_mutex_t *mutex);
    int pt_mutex_destroy(pt_mutex_t *mutex);


#endif
