
#include <stdio.h> //funciones de input/output (printf, scanf)
#include <stdlib.h> //funciones generales (atoi, exit)
#include "../input_manager/manager.h" //funciones de manejo de input

#include <string.h> //funciones de manejo de cadenas de caracteres (strcmp)
#include <unistd.h> //funciones de sistema (fork, execvp)
#include <sys/wait.h>   //funciones de espera (waitpid)
#include <signal.h>     //manejo de señales (signal)
#include "main.h" // header con definiciones
#include <pthread.h>

ProcessInfo processes[MAX_PROCESSES]; //array de procesos
int process_count = 0; //contador de procesos
int time_max_limit = -1; //limite de tiempo (-1 = ilimitado)

static bool string_equals(char *string1, char *string2) { 
  return !strcmp(string1, string2); //compara dos strings, true si son iguales
}

void *check_timeout_thread(void *arg) {
  while (1) {
    check_process_timeouts();
    sleep(1);
  }
  return NULL;
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
    processes[process_count].timeout_start = 0;
    processes[process_count].exit_code = 0;
    processes[process_count].signal_received = 0;
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
      processes[i].status,
      processes[i].exit_code,
      processes[i].signal_received);
  }
}

int main(int argc, char const *argv[])
{

  if (argc == 1) {
    time_max_limit = -1;
    printf("DCControl iniciado sin tiempo maximo\n");
  } else if (argc == 2) {
    time_max_limit = atoi(argv[1]); //se convierte el tiempo maximo a un entero
    printf("time_max_limit: %d\n", time_max_limit);
    if (time_max_limit <= 0) { //si el tiempo maximo es menor o igual a 0, se imprime un error
      printf("ERROR: El tiempo máximo debe ser mayor que 0\n");
      return 1;
    }
    printf("DCControl inciado con tiempo maximo de %d segundos\n", time_max_limit);
  } else {
    printf("Uso: %s [<tiempo_maximo>]\n", argv[0]);
    return 1;
  }
  set_buffer(); // No borrar 

  signal(SIGCHLD, signal_handler);

  pid_t timeout_pid

  
  while (1) {

    char** input = read_user_input();

    
    if (string_equals(input[0], "launch")) {
      printf("input[1]: %s\n", input[1]);
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


