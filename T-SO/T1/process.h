#ifndef PROCESS_H
#define PROCESS_H

typedef enum {
     RUNNING,
     READY,
     WAITING,
     FINISHED,
     DEAD
} State;

typedef struct {
    char name[64];      //nombre del proceso
    int pid;            //process ID
    State state;        //estado del proceso
    int burst_time;     //tiempo de CPU requerido
    int num_bursts;     //número de ráfagas
    int bursts_done;    //ráfagas completadas
    int io_wait;        //ticks de espera entre rafagas
    int deadline;       //deadline del proceso
    int start_time;     //tiempo en que el proceso llega al sistema
    int last_cpu_exit;  //último tick en que el proceso salió de la CPU
    double priority;    //prioridad del proceso
    int interruptions;  //número de interrupciones sufridas
    int first_response; //tiempo en que el proceso obtiene la CPU por primera vez
    int finish_time;    //tiempo en que el proceso finaliza su ejecución
    int waiting_time;   //acumulado en READY/WAITING
    int quantum_used;   //ticks usados del quantum actual
    int burst_remaining; //ticks restantes de la ráfaga actual
    int io_remaining;    //ticks restantes de espera de I/O
} Process;
 
#endif