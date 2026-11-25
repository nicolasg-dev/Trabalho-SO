#include <stdio.h>
#include <stdlib.h>
#include "platform_thread.h"


#ifdef _WIN32

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

DWORD Sum; /* os dados são compartilhados pelo(s) thread(s) */

/* o thread é executado nessa função separada */
DWORD WINAPI Summation(LPVOID Param)
{
    DWORD Upper = *(DWORD*)Param;

    for (DWORD i = 0; i <= Upper; i++)
        Sum += i;

    return 0;
}

int main(int argc, char *argv[])
{
    DWORD ThreadId;
    HANDLE ThreadHandle;
    int Param;

    if (argc != 2) {
        fprintf(stderr, "An integer parameter is required\n");
        return -1;
    }

    Param = atoi(argv[1]);

    if (Param < 0) {
        fprintf(stderr, "An integer >= 0 is required\n");
        return -1;
    }

    /* cria o thread */
    ThreadHandle = CreateThread(
        NULL,       /* atributos default de segurança */
        0,          /* tamanho default de pilha */
        Summation,  /* função do thread */
        &Param,     /* parâmetro para a função do thread */
        0,          /* flags default de criação */
        &ThreadId   /* retorna o identificador do thread */
    );

    if (ThreadHandle != NULL) {
        /* agora espera que o thread seja encerrado */
        WaitForSingleObject(ThreadHandle, INFINITE);

        /* fecha o manipulador do thread */
        CloseHandle(ThreadHandle);

        printf("sum = %lu\n", (unsigned long)Sum);
    }

    return 0;
}



#else

volatile int queimaCPU;
int contador;
int duration;
pt_mutex_t mutex;

void* runner(void *param);

// To compile:  gcc main.c platform_thread.c -o main -pthread
// ./main "n° threads" "duração da região crítica (1 - curta, 2 - media, 3 - longa)"
                  
int main (int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr,"usage: a.out <integer value>\n");

        return -1;        
    }

    int N = atoi(argv[1]);
    duration = atoi(argv[2]);
    //char* primitiva = "mutex";
    contador = 0;

    pthread_t Atids[N];
    pthread_t tid;
    pt_mutex_init(&mutex);    


    for (int i = 0; i < N; i++){
        if (pthread_create(&tid, NULL, runner, argv[1]))
            fprintf(stderr, "Erro na criação da thread. \n");
        //sleep(10);

        Atids[i] = tid;
    }

    for (int i = 0; i < N; i++){
        pthread_join(Atids[i], NULL);
    }

    pt_mutex_destroy(&mutex);
    printf("Contador: %d\n", contador);
    return 0;
}


void *runner(void *param)
{
    int i, upper = atoi(param);
    printf("Nova thread!\n");

    switch (duration) {
        case 1:
            queimaCPU = 0;
            break;
        case 2:
            queimaCPU = 2000;
            break;
        case 3:
            queimaCPU = 3000;
            break;
        default:
            printf("Invalid option.\n");
    }

    for (int i = 0; i < 30; i++){
        pt_mutex_lock(&mutex);
        contador++;
        while(queimaCPU > 0){
            queimaCPU--;
        }
        pt_mutex_unlock(&mutex);
    }


    pthread_exit(0);
}


#endif

