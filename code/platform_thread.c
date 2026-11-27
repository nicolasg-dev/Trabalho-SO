#include <stdio.h>
#include "platform_thread.h"

#ifdef _WIN32

    int pt_clock(unsigned long long *tempo){
        //unsigned long long resto = 0;
        LARGE_INTEGER atual, frequencia;

        QueryPerformanceFrequency(&frequencia);

        QueryPerformanceCounter(&atual);

        //resto = atual.QuadPart
        //            % frequencia.QuadPart;
        
        *tempo = atual.QuadPart
                    / frequencia.QuadPart;
                    
        *tempo = (*tempo) * 1000000000;
        return 0;
    }

    int pt_join(pt_t thread){
        return WaitForSingleObject(thread, INFINITE);
    }
/* 
    int pt_create(LPDWORD *tid
        , void* attr
        , LPTHREAD_START_ROUTINE function
        , LPVOID param) */
    int pt_create(pt_t thread, 
        DWORD (*start_routine)(void *), void *arg){
        thread = CreateThread(NULL, 0, start_routine, arg, 0, thread);
        if (thread != NULL) return 0;
        return 1;
    }
    //pthread_create(&tid, NULL, runner, argv[1])
    int pt_mutex_init(pt_mutex_t *mutex){
        if (InitializeCriticalSectionAndSpinCount(mutex, 0x00000400))return 0;
        else return -1;
    }

    int pt_mutex_lock(pt_mutex_t *mutex){
        EnterCriticalSection(mutex);
        return 0;
    }

    int pt_mutex_trylock(pt_mutex_t *mutex){
        if (TryEnterCriticalSection(mutex)) return 0;
        else return -1;
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

    double pt_clock(int (*benchmark)(char**, int)){

        LARGE_INTEGER inicio, fim, frequencia;

        QueryPerformanceFrequency(&frequencia);

        QueryPerformanceCounter(&inicio);

        benchmark;

        QueryPerformanceCounter(&fim);

        double tempo = (double)(fim.QuadPart - inicio.QuadPart)
                    / frequencia.QuadPart;

        printf("Tempo: %f segundos\n", tempo);

        return tempo;

    }
    int pt_join(pt_t thread){
        return pthread_join(thread, NULL);
    }

    int pt_create(pt_t thread, 
        DWORD (*start_routine)(void *), void *arg){
        return pthread_create(&thread, NULL, (void*)start_routine, arg);
    }

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

