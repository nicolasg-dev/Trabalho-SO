#include <stdio.h>
#include <stdlib.h>
#include "platform_thread.h"



// Compilar (Windows):  gcc main.c benchmark.c platform_thread.c -o main.exe
// Compilar (Linux):  gcc main.c benchmark.c platform_thread.c -o main -pthread 
// ./main "n° de threads" "duração da região crítica (1 - curta, 2 - media, 3 - longa)"
  
int main (int argc, char* argv[]){
    if (argc != 3){
        fprintf(stderr,"usage: <this needs 3 args>\n");
        return -1;        
    }
    if (atoi(argv[1]) <= 0) {
        fprintf(stderr, "An integer > 0 is required\n");
        return -1;
    }
    

    for (int i = 0; i < 5; i++){
        if(benchmark(argv)){
            fprintf(stderr, "Erro em algum benchmark \n");
        }
    }


    return 0;
}