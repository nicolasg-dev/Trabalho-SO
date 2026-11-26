#include <stdio.h>
#include <stdlib.h>
#include "platform_thread.h"

volatile int queimaCPU;
int temp, N, T;
unsigned long contador;
int duration;
pt_mutex_t mutex;

DWORD runner(LPVOID param);

// Compilar (Windows):  gcc main.c platform_thread.c -o main.exe
// Compilar (Linux):  gcc main.c platform_thread.c -o main -pthread 
// ./main "n° de threads" "duração da região crítica (1 - curta, 2 - media, 3 - longa)"
                  
int main (int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr,"usage: a.out <this needs 3 args>\n");
        return -1;        
    }

    N = atoi(argv[1]);
    T = 4096;
    if (N <= 0) {
        fprintf(stderr, "An integer > 0 is required\n");
        return -1;
    }    
    duration = atoi(argv[2]); // 1 ou 2 ou 3.
        switch (duration) {
            case 1:
                queimaCPU = 0; // curta
                break;
            case 2:
                queimaCPU = 10000; // media
                break;
            case 3:
                queimaCPU = 1000000; // longa
                break;
            default:
                printf("Invalid option.\n");
                return -1;
        }
    temp = queimaCPU;
    //char* primitiva = "mutex";
    contador = 0;

    pthread_t Atids[N];
    pthread_t tid;
    pt_mutex_init(&mutex); // InitializeCriticalSectionAndSpinCount


    for (int i = 0; i < N; i++){
        if (pt_create(tid, runner, argv[1])) // CreateThread
            fprintf(stderr, "Erro na criação da thread. \n");

        Atids[i] = tid;
    }

    for (int i = 0; i < N; i++){
        pt_join(Atids[i]); // WaitForSingleObject
    }

    pt_mutex_destroy(&mutex);
    printf("Contador: %lu\n", contador);
    return 0;
}


DWORD runner(LPVOID param)
{
    printf("Thread;\n");
    
    for (int i = 0; i < (int)T/N; i++){
        pt_mutex_lock(&mutex);
        queimaCPU = temp;
        
        contador++;
        while(queimaCPU > 0){
            queimaCPU--;
        }
        pt_mutex_unlock(&mutex);
    }
    return 1;
}

