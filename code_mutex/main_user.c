#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "platform_thread.h"

int benchmark (char *argv[], int repeticao);
unsigned long long tempo_inicial, tempo_final;

// Compilar (Windows):  gcc main_user.c benchmark.c platform_thread.c -o main.exe
// Compilar (Linux):  gcc main_user.c benchmark.c platform_thread.c -o main -pthread 
// ./main "n° threads" "duração da região crítica (1 - curta, 2 - media, 3 - longa)" "mutex/spin-lock"

int main (int argc, char* argv[]){
    if (argc != 4){
        fprintf(stderr,"usage: <this needs 4 args>\n");
        return -1;        
    }
    if (atoi(argv[1]) <= 0) {
        fprintf(stderr, "An integer > 0 is required\n");
        return -1;
    }
     
    for (int i = 1; i <= 30; i++){
        if(benchmark(argv, i)){
            fprintf(stderr, "Erro em algum benchmark!\n");
        }
    }
     

/*     pt_clock(&tempo_inicial);
    sleep(2);
    pt_clock(&tempo_final);

    printf("%llu\n", tempo_final - tempo_inicial); */
    return 0;
}