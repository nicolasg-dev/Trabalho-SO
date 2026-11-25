#include <stdio.h>
#include "platform_thread.h"

// To compile:  gcc main.c platform_thread.c -o main -pthread
int main (){
    pt_mutex_t *mutex;

    pt_mutex_init(mutex);
    pt_mutex_lock(mutex);
    pt_mutex_unlock(mutex);
    pt_mutex_destroy(mutex);

    



    return 0;
}