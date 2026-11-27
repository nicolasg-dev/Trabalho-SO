#include <stdio.h>
#include <stdlib.h>
#include "platform_thread.h"

volatile int queimaCPU;
int temp, N, T, duration;
unsigned long contador_final, contador_esperado;
unsigned long long t_inicial, t_final;
pt_mutex_t mutex;

DWORD runner(LPVOID param);
                
int benchmark (char *argv[], int repeticao){
    FILE *arquivo = fopen("dados_brutos.csv", "a");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    N = atoi(argv[1]);
    T = 4096;
    contador_esperado = N * T/N;
    char* strDuration;
    duration = atoi(argv[2]); // 1 ou 2 ou 3.
        switch (duration) {
            case 1:
                queimaCPU = 0; // curta
                strDuration = "curta";
                break;
            case 2:
                queimaCPU = 10000; // media
                strDuration = "media";
                break;
            case 3:
                queimaCPU = 1000000; // longa
                strDuration = "longa";
                break;
            default:
                printf("Invalid option.\n");
                return -1;
        } 
    temp = queimaCPU;
    contador_final = 0;

    pt_t Atids[N];
    pt_t tid;
    pt_mutex_init(&mutex); // InitializeCriticalSectionAndSpinCount

    pt_clock(&t_inicial);

    for (int i = 0; i < N; i++){
        if (pt_create(tid, runner, argv[1])) // CreateThread
            fprintf(stderr, "Erro na criação da thread. \n");

        Atids[i] = tid;
    }

    for (int i = 0; i < N; i++){
        pt_join(Atids[i]); // WaitForSingleObject
    }

    pt_clock(&t_final);

    pt_mutex_destroy(&mutex);
    
    #ifdef _WIN32
        fprintf(arquivo, "Windows,%s,%d,%s,%llu,%d,%lu,%lu\n", argv[3], N, strDuration, t_final - t_inicial, repeticao, T, contador_final, contador_esperado);
    #else
        fprintf(arquivo, "Linux,%s,%d,%s,%llu,%d,%lu,%lu\n", argv[3], N, strDuration, t_final - t_inicial,repeticao, T, contador_final, contador_esperado);
    #endif 
    //plataforma,primitiva,N,duracao,tempo_bruto,repeticao,T,contador_final,contador_esperado

    fclose(arquivo);
    return 0;
}


DWORD runner(LPVOID param)
{
    
    for (int i = 0; i < T/N; i++){
        pt_mutex_lock(&mutex);
        queimaCPU = temp;
        
        contador_final++;
        while(queimaCPU > 0){
            queimaCPU--;
        }
        pt_mutex_unlock(&mutex);
    }
    return 1;
}