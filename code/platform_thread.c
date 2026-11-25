#include <stdio.h>
#include "platform_thread.h"

#ifdef _WIN32
    int pt_mutex_init(pt_mutex_t *mutex){
        if (InitializeCriticalSectionAndSpinCount(mutex, 0x00000400))return 0;
        else return 1;
    }

    int pt_mutex_lock(pt_mutex_t *mutex){
        EnterCriticalSection(mutex);
        return 0;
    }

    int pt_mutex_trylock(pt_mutex_t *mutex){
        if (TryEnterCriticalSection(mutex)) return 0;
        else return 1;
    }

    int pt_mutex_unlock(pt_mutex_t *mutex){
        LeaveCriticalSection(mutex);
        return 0;
    }

    int pt_mutex_destroy(pt_mutex_t *mutex){
        DeleteCriticalSection(mutex);
        return 0;
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

