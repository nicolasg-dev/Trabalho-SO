#include <stdio.h>
#include <stdlib.h>
#include "platform_thread.h"

volatile int queimaCPU;
int temp, N, T, duration;
unsigned long contador;
pt_mutex_t mutex;

DWORD runner(LPVOID param);
                
int benchmark (char *argv[], int qual){
    FILE *arquivo = fopen("dados_brutos.csv", "a");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    N = atoi(argv[1]);
    T = 4096;

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

    pt_t Atids[N];
    pt_t tid;
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
    
    #ifdef _WIN32
        fprintf(arquivo, "Windows,%s,%d,%d,%d,tempo_bruto,%lu,%d\n", argv[3], N, duration, qual, T, contador, N*T/N);
    #else
        fprintf(arquivo, "Linux,%s,%d,%d,%d,tempo_bruto,%lu,%d\n", argv[3], N, duration, qual, T, contador, N*T/N);
    #endif 
    //plataforma,primitiva,N,duracao,repeticao,tempo_bruto,T,contador_final,contador_esperado

    fclose(arquivo);
    return 0;
}


DWORD runner(LPVOID param)
{
    
    for (int i = 0; i < T/N; i++){
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