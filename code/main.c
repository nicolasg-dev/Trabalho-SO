#include <stdio.h>
#include <stdlib.h>
#include "platform_thread.h"

int benchmark (char *argv[], int qual);

// Compilar (Windows):  gcc main.c benchmark.c platform_thread.c -o main.exe
// Compilar (Linux):  gcc main.c benchmark.c platform_thread.c -o main -pthread 
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
    
    FILE *arquivo = fopen("dados_brutos.csv", "a");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
    double tempo_bruto;
    for (int i = 1; i <= 30; i++){
        benchmark(argv, i);
        tempo_bruto = pt_time_now(benchmark);
        fprintf(arquivo, "%f\n", tempo_bruto);
    }
    
    fclose(arquivo);

    return 0;
}