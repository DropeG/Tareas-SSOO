
#include <stdio.h>
#include <stdlib.h>
#include "../input_manager/manager.h"

#include <string.h>
#include <unistd.h>
#include <sys/wait.h>   
#include <signal.h>     
#include "main.h"

ProcessInfo processes[MAX_PROCESSES];
int process_count = 0;


static bool string_equals(char *string1, char *string2) {
  return !strcmp(string1, string2);
}

void launch_process(char *executable, char **args) {
  if (process_count >= MAX_PROCESSES) {
    printf("ERROR: Máximo de procesos alcanzado\n");
    return;
  }

  pid_t pid = fork();
  if (pid == 0) { // Hijo
    execvp(executable, args);
    perror("Error al ejecutar el proceso");
    exit(1);
  } else if (pid > 0) { // Padre 
    processes[process_count].pid = pid;
    strcpy(processes[process_count].name, executable);
    processes[process_count].start_time = time(NULL);
    processes[process_count].status = 0;
    process_count++;
    printf("Proceso %s lanzado con PID %d\n", executable, pid);
  } else {
    printf("Error al lanzar el proceso\n");
  }
}

void show_status() {
  printf("PID\tNombre\t\tTiempo\t\tEstado\t\tExit Code\tSignal\n");
  for (int i = 0; i < process_count; i++) {
    time_t elapsed = time(NULL) - processes[i].start_time;
    printf("%d\t%s\t\t%lds\t\t%d\t\t%d\t\t%d\n", 
      processes[i].pid, 
      processes[i].name, 
      elapsed,
      processes[i].status = 0,
      processes[i].exit_code,
      processes[i].signal_received);
  }
}



int main(int argc, char const *argv[])
{
  set_buffer(); // No borrar

  while (1) {
    char** input = read_user_input();
    if (string_equals(input[0], "launch")) {
      if (input[1] != NULL) {
        launch_process(input[1], &input[1]);
      } else{
        printf("ERROR: Debe especificar el nombre del proceso\n");
      }
    } else if (string_equals(input[0], "status")) {
      show_status();
    } else if (string_equals(input[0], "kill")) {
      printf("COMANDO KILL\n");
    } else if (string_equals(input[0], "abort")){
      printf("COMANDO ABORT\n");
    } else if (string_equals(input[0], "shutdown")) {
      printf("COMANDO SHUTDOWN\n");
    } else if (string_equals(input[0], "emergency")) {
      printf("COMANDO EMERGENCY\n");
    } else if (string_equals(input[0], "exit")) {
      printf("COMANDO EXIT\n");
      free_user_input(input);
      break;
    } else {
      printf("ERROR: Comando no válido\n");
    }
    free_user_input(input);
  }

  return 0;

}


