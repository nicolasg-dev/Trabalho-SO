#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "platform_thread.h"

int benchmark (char *argv[], int repeticao);
unsigned long long tempo_inicial, tempo_final;

// Compilar (Windows):  gcc main_bench.c benchmark.c platform_thread.c -o main.exe
// Compilar (Linux):  gcc main_bench.c benchmark.c platform_thread.c -o main -pthread 
// ./main "n° threads" "duração da região crítica (1 - curta, 2 - media, 3 - longa)" "mutex/spin-lock"

int bench (char* argv[]);

int main(){
    char buf0[16];
    char buf1[16];
    char *args[3] = {buf0, buf1, NULL};

    for (int i = 1; i <= 16; i*=2){
        for (int j = 1; j <= 3; j++){
            snprintf(buf0, sizeof(buf0), "%d", i);
            snprintf(buf1, sizeof(buf1), "%d", j);
            for (int rep = 1; rep <= 30; rep++){
                if(benchmark(args, rep)){
                    fprintf(stderr, "Erro em algum benchmark!\n");
                }
            }            
/*             for (int k = 1; k <= 2; k++){

            } */
        }
    }
    return 0;
}