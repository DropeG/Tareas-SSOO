#include <stdio.h>
#include <stdlib.h>
#include "../input_manager/manager.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>   
#include <signal.h>     
#include "main.h"
#include <sys/time.h>

typedef enum {
    RUNNING,  //indica que el proceso esta ejecutando dentro de la CPU. Solo puede haber un proceso en este estado a la vez
    READY,    //indica que el proceso esta listo para ejecutar
    WAITING,  //indica que el proceso esta esperando y no puede ejecutar
    FINISHED, //indica que el proceso ya ha terminado su ejecucion
    DEAD      //indica que el proceso no pudo terminar su ejecucion (bursts) en su deadline
}Estado;

typedef enum {
    HIGH,
    LOW
} QueueType;

typedef struct {
    char name[32];
    int pid;
    Estado state;
    int burst_time;
    int num_bursts;   
    int io_wait;
    int deadline;
}Proceso;

typedef struct {
    Proceso* processes;
    int size;
    int capacity;
    int quantum;
    QueueType type;
} Queue;



int main() {
    printf("Hola, soy el programa DCCambios!\n");
    return 0;
}