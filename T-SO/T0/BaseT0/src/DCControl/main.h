#ifndef MAIN_H  // Si MAIN_H no está definido, define MAIN_H
#define MAIN_H

#define MAX_PROCESSES 10 // Máximo de procesos que se pueden ejecutar

#include <sys/types.h>  // pid_t
#include <time.h> // time_t
#include <stdbool.h>

typedef struct  {
    pid_t pid;  // PID del proceso
    char name[128]; // Nombre del proceso
    int status; // Estado del proceso (0: running, 1: stopped, 2: terminated)
    time_t start_time; // Tiempo de inicio del proceso
    int exit_code;
    int signal_received;
} ProcessInfo;

extern ProcessInfo processes[MAX_PROCESSES];
extern int process_count;

int add_process(const char* name);
int remove_process(pid_t pid);
void list_processes(void);
void kill_process(pid_t pid);
static bool string_equals(char *string1, char *string2);

#endif