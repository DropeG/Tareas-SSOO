#ifndef MAIN_H
#define MAIN_H

#define MAX_PROCESSES 10

#include <sys/types.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    pid_t pid;
    char name[128];
    int status; // 0: running, 1: stopped, 2: terminated
    time_t start_time;
    time_t timeout_start;
    int exit_code;
    int signal_received;
} ProcessInfo;

extern ProcessInfo processes[MAX_PROCESSES];
extern int process_count;

void launch_process(char *executable, char **args);
void show_status(void);

#endif