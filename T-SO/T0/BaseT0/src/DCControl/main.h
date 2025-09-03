#ifndef MAIN_H
#define MAIN_H

#define MAX_PROCESSES 10

#include <sys/types.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    pid_t pid;
    char name[128];
    int status; // 0: running, 1: stopped, 2: terminated, 3: timeout_warning, 4: force_terminated
    time_t start_time;
    time_t end_time;
    time_t timeout_start;
    int exit_code;
    int signal_received;
} ProcessInfo;

extern ProcessInfo processes[MAX_PROCESSES];
extern int process_count;

void launch_process(char *executable, char **args);
void show_status(void);
void check_process_timeouts(void);
void timer_handler(int sig);
void setup_timer(void);
void abort_process(int time_seconds);
void print_final_statistics();
void shotdown_process();
void emergency_process();
void print_table_header(void);

#endif