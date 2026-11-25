#include <stdio.h>
#include "platform_thread.h"

#ifdef _WIN32
    int pt_mutex_init(pt_mutex_t *mutex){
        return InitializeCriticalSectionAndSpinCount(mutex, 0x00000400);
    }

    int pt_mutex_lock(pt_mutex_t *mutex){
        return EnterCriticalSection(mutex);
    }

    int pt_mutex_trylock(pt_mutex_t *mutex){
        return TryEnterCriticalSection(mutex);
    }

    int pt_mutex_unlock(pt_mutex_t *mutex){
        return LeaveCriticalSection(mutex);
    }

    int pt_mutex_destroy(pt_mutex_t *mutex){
        return DeleteCriticalSection(mutex);
    }
#else

    int pt_mutex_init(pt_mutex_t *mutex){
        return pthread_mutex_init(mutex, NULL);
    }

    int pt_mutex_lock(pt_mutex_t *mutex){
        return pthread_mutex_lock(mutex);
    }

    int pt_mutex_trylock(pt_mutex_t *mutex){
        return pthread_mutex_trylock(mutex);
    }

    int pt_mutex_unlock(pt_mutex_t *mutex){
        return pthread_mutex_unlock(mutex);
    }

    int pt_mutex_destroy(pt_mutex_t *mutex){
        return pthread_mutex_destroy(mutex);
    }


#endif

