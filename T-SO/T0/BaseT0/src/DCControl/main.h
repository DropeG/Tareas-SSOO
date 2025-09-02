#ifndef MAIN_H  // Si MAIN_H no está definido, define MAIN_H
#define MAIN_H

#define MAX_PROCESSES 10 // Máximo de procesos que se pueden ejecutar

#include <sys/types.h>  // pid_t
#include <sys/time.h> // time_t
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>

typedef struct  {
    pid_t pid;  // PID del proceso
    char name[128]; // Nombre del proceso
    int status; // Estado del proceso (0: running, 1: stopped, 2: terminated, 3: timeout_warning, 4: force_terminated)
    time_t start_time; // Tiempo de inicio del proceso
    time_t timeout_start; // Tiempo de inicio de la advertencia de timeout
    int exit_code;
    int signal_received;
    pthread_mutex_t mutex;
} ProcessInfo;

extern ProcessInfo processes[MAX_PROCESSES];
extern int process_count;
extern int time_max_limit;

int add_process(const char* name);
int remove_process(pid_t pid);
void list_processes(void);
void kill_process(pid_t pid);
static bool string_equals(char *string1, char *string2);
void launch_process(char *executable, char **args);
void show_status(void);
void update_process_status(pid_t pid, int status);
void check_process_timeouts(void);
void signal_handler(int sig);

#endif