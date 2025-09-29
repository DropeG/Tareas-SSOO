
#include <stdio.h>
#include <stdlib.h>
#include "../input_manager/manager.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>   
#include <signal.h>     
#include "main.h"
#include <sys/time.h>

ProcessInfo *processes = NULL;
int process_capacity = 0;
int process_count = 0;
int active_count = 0;
int time_max_limit = -1; 

int abort_active = 0;
time_t abort_start_time = 0;
int abort_duration = 0; 

int shutdown_active = 0;
time_t shutdown_start_time = 0;
int shutdown_duration = 10; 

int contador = 0;

void cleanup() {
  if (processes != NULL) {
    free(processes);
    processes = NULL;
  }
}

void expand_process_array() {
  if (process_count >= process_capacity) {
    int new_cap = (process_capacity == 0) ? 10 : process_capacity * 2;

    ProcessInfo *tmp = realloc(processes, new_cap * sizeof(ProcessInfo));
    if (tmp == NULL) {
      perror("realloc");       // imprime razón del fallo
      free(processes);         // libera el bloque original para no filtrar
      processes = NULL;
      exit(1);
    }

    processes = tmp;
    process_capacity = new_cap;
  }
}

void print_table_header(void) {
  printf("%-6s %-20s %-10s %-8s %-8s\n",
         "PID", "Nombre", "Tiempo", "Exit", "Signal");
}

void print_final_statistics() {
  printf("DCControl finalizado.\n");
  print_table_header();

  for (int i = 0; i < process_count; i++) {
    time_t execution_time;

    if (processes[i].status == 0) {
      execution_time = time(NULL) - processes[i].start_time;
    } else {
      execution_time = processes[i].end_time - processes[i].start_time;
    }

    printf("%-6d %-20s %ld%-9s %-8d %-8d\n",
           processes[i].pid,
           processes[i].name,
           execution_time,
           "s",
           processes[i].exit_code,
           processes[i].signal_received);
  }
}

void timer_handler(int sig) {
  if (sig == SIGALRM) {
    contador++;
    check_process_timeouts();
    signal(SIGALRM, timer_handler);
  }
}

void signal_handler(int sig) {
  if (sig == SIGCHLD) {
    int status;
    pid_t pid;

    
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
      for (int i = 0; i<process_count; i++) {
        if (processes[i].pid == pid) {
          processes[i].end_time = time(NULL);

          if (WIFEXITED(status)) {
            processes[i].status = 2; // terminated
            processes[i].exit_code = WEXITSTATUS(status);
            printf("✅ Proceso %s (PID %d) terminó con código %d\n", 
                               processes[i].name, pid, processes[i].exit_code);
          } else if (WIFSIGNALED(status)) {
            processes[i].status = 2;
            processes[i].signal_received  = WTERMSIG(status);
            printf("⚠️ Proceso %s (PID %d) terminó por señal %d\n", 
                               processes[i].name, pid, processes[i].signal_received);
          }
          active_count--;
          break;
        }
      }
    }
  }
}

void check_process_timeouts() {
  if (time_max_limit == -1) {
    return;
  }

  time_t current_time = time(NULL);

  if (shutdown_active) {
    time_t elapsed_shutdown = current_time - shutdown_start_time;

    if (elapsed_shutdown >= shutdown_duration) {
      for (int i = 0; i < process_count; i++) {
        if (kill(processes[i].pid, 0) == 0) {
          printf("�� Enviando SIGKILL a PID %d (%s)\n", processes[i].pid, processes[i].name);
          kill(processes[i].pid, SIGKILL);
          processes[i].status = 4; // force_terminated
          processes[i].end_time = time(NULL);
          processes[i].signal_received = 9;
        }
      }

      print_final_statistics();
      cleanup();
      exit(0);
    }
    return;
  }

  if (abort_active) {
    time_t elapsed_abort = current_time - abort_start_time;

    if (elapsed_abort >= abort_duration) {
      printf("Abort cumplido.\n");
      print_table_header();

      for (int i = 0; i < process_count; i++) {
        if (processes[i].status == 0) {
          time_t execution_time = current_time - processes[i].start_time;
          printf("%-6d %-20s %ld%-9s %-8d %-8d\n",
            processes[i].pid,
            processes[i].name,
            execution_time,
            "s",
            processes[i].exit_code,
            processes[i].signal_received);

          kill(processes[i].pid, SIGTERM);
          processes[i].status = 3; // timeout_warning
          processes[i].timeout_start = current_time;
        }
      }

      abort_active = 0;
      printf("✅ Abort completado\n");
    }
    return;
  } 

  for (int i = 0; i<process_count; i++) {
    if (processes[i].status == 0) {
      time_t elapsed = current_time - processes[i].start_time;
    
      if (elapsed >= time_max_limit) {
        printf("⚠️ El proceso debe terminar en 5 segundos como maximo\n");
        kill(processes[i].pid, SIGTERM);
        processes[i].status = 3; // timeout_warning
        processes[i].timeout_start = current_time;
        printf("   📤 Enviando SIGTERM a PID %d\n", processes[i].pid);
      }
    }

    if (processes[i].status == 3) {
      time_t time_since_sigterm = current_time - processes[i].timeout_start;

      if (kill(processes[i].pid, 0) == 0) {  // Proceso sigue vivo
        if (time_since_sigterm >= 5) {
            printf("⚠️ Proceso %s (PID %d) forzado a terminar con SIGKILL\n", 
                   processes[i].name, processes[i].pid);
            kill(processes[i].pid, SIGKILL);
            processes[i].status = 4; // force_terminated
        }
      } else {
        printf("✅ Proceso %s terminó naturalmente después de SIGTERM\n", 
          processes[i].name);
          processes[i].status = 2; // terminated
      }
    }
  }

}

void setup_timer() {
  if (time_max_limit == -1) {
    return;
  }

  struct itimerval timer;
  timer.it_interval.tv_sec = 1;
  timer.it_interval.tv_usec =0;
  timer.it_value.tv_sec = 1;
  timer.it_value.tv_usec = 0;

  setitimer(ITIMER_REAL, &timer, NULL);

}

static bool string_equals(char *string1, char *string2) { 
  return !strcmp(string1, string2);
}

void launch_process(char *executable, char **args) {
  if (active_count >= MAX_PROCESSES) {
    printf("ERROR: Máximo de procesos alcanzado\n");
    return;
  }

  expand_process_array();

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
    processes[process_count].exit_code = -1;
    processes[process_count].signal_received = -1;
    processes[process_count].end_time = 0;
    process_count++;
    active_count++;
    printf("✅ Proceso %s lanzado con PID %d (start_time: %ld)\n", 
           executable, pid, processes[process_count-1].start_time);
  } else {
    printf("Error al lanzar el proceso\n");
  }
}

void show_status() {
  print_table_header();
  for (int i = 0; i < process_count; i++) {
    time_t execution_time;

    if (processes[i].status == 0) {
      execution_time = time(NULL) - processes[i].start_time;
    } else {
      execution_time = processes[i].end_time - processes[i].start_time;
    }
    printf("%-6d %-20s %ld%-9s %-8d %-8d\n",
      processes[i].pid,
      processes[i].name,
      execution_time,
      "s",
      processes[i].exit_code,
      processes[i].signal_received);
  }
}

void abort_process(int time_seconds) {
  printf("🔄 Comando ABORT iniciado con tiempo: %d segundos\n", time_seconds);

  int running_processes = 0;
  for (int i = 0; i < process_count; i++) {
    if (processes[i].status == 0) {
      running_processes++;
    }
  }

  printf("📊 Procesos en ejecución encontrados: %d\n", running_processes);

  if (running_processes == 0) {
    printf("❌ No hay procesos en ejecución. Abort no se puede ejecutar.\n");
    return;
  }

  abort_active = 1;
  abort_start_time = time(NULL);
  abort_duration = time_seconds;

  printf("⏰ Abort activado. Esperando %d segundos...\n", time_seconds);
  printf("💡 El abort se completará automáticamente en %d segundos\n", time_seconds);

}

void shotdown_process() {
  printf("🔄 Comando SHUTDOWN iniciado\n");

  int running_processes = 0;
  for(int i = 0; i < process_count; i++) {
    if (processes[i].status == 0) {
      running_processes++;
    }
  }

  printf("📊 Procesos en ejecución encontrados: %d\n", running_processes);

  if (running_processes == 0) {
    print_final_statistics();
    cleanup();
    exit(0);
  }

  printf("⚠️ Hay procesos en ejecución. Enviando SIGINT...\n");

  for (int i = 0; i < process_count; i++) {
    if (processes[i].status == 0) {
      printf("📤 Enviando SIGINT a PID %d (%s)\n", processes[i].pid, processes[i].name);
      kill(processes[i].pid, SIGINT);
      processes[i].status = 3; // timeout_warning
      processes[i].timeout_start = time(NULL);
    }
  }

  shutdown_active = 1;
  shutdown_start_time =time(NULL);

  printf("⏰ Shutdown activado. Esperando 10 segundos...\n");
  printf("💡 El shutdown se completará automáticamente en 10 segundos\n");
  printf("⚠️ Cualquier comando abort queda anulado\n");

  abort_active = 0;
  
}

void emergency_process() {
  printf("¡Emergencia!\n");

  int running_processes = 0;
  for (int i = 0; i < process_count; i++) {
    if (processes[i].status == 0) {
      running_processes++;
    }
  }

  printf("📊 Procesos en ejecución encontrados: %d\n", running_processes);

  if (running_processes == 0) {
    printf("✅ No hay procesos en ejecución.\n");
  } else {
    printf("⚠️ Enviando SIGKILL a todos los procesos...\n");

    for (int i = 0; i < process_count; i++) {
      if (processes[i].status == 0) {
        printf(" Enviando SIGKILL a PID %d (%s)\n", processes[i].pid, processes[i].name);
        kill(processes[i].pid, SIGKILL);
        processes[i].status = 4; // force_terminated
        processes[i].end_time = time(NULL);
        processes[i].signal_received = 9;
      }
    }
  }

  print_final_statistics();
  cleanup();
  exit(0);
}

int main(int argc, char const *argv[])
{
  if (argc == 1) { // si no hay argumentos, entonces no hay tiempo maximo
    time_max_limit = -1;
    printf("DCControl inciado sin tiempo maximo\n");
  } else if (argc == 2) { //si hay un argumento, entonces hay un tiempo maximo (./DCControl <arg>)
    time_max_limit = atoi(argv[1]);
    if ( time_max_limit <= 0 ){
      printf("ERROR: Tiempo tiempo maximo tiene que ser mayor que 0\n");
      return 1;
    }
    printf("DCControl iniciado con tiempo maximo de %d segundos\n", time_max_limit);
  } else {
    printf("Uso: ./DCControl <tiempo_maximo>\n");
    return 1;
  }

  set_buffer(); // No borrar
  signal(SIGCHLD, signal_handler); 
  signal(SIGALRM, timer_handler); 
  setup_timer(); 

  printf("🔄 Sistema listo. Esperando comandos...\n\n");

  while (1) {
    char** input = read_user_input();
    
    if (string_equals(input[0], "launch")) {
      if (input[1] != NULL) {
        launch_process(input[1], &input[1]);
      } else {
        printf("❌ ERROR: Debe especificar el nombre del comando\n");
      }
    } else if (string_equals(input[0], "status")) {
      show_status();
    } else if (string_equals(input[0], "abort")) {
      if (shutdown_active) {
        printf("⚠️ Shutdown activo. Comando abort anulado.\n");
      } else if (input[1] != NULL) {
        int time_seconds = atoi(input[1]);
        if (time_seconds > 0) {
          abort_process(time_seconds);
        } else {
          printf("❌ ERROR: El tiempo debe ser mayor que 0\n");
        }
      } else {
        printf("❌ ERROR: Debe especificar el tiempo en segundos\n");
      }
    } else if (string_equals(input[0], "shutdown")) {
      free_user_input(input);
      input = NULL;
      shotdown_process();
    } else if (string_equals(input[0], "emergency")) {
      free_user_input(input);
+     input = NULL;
      emergency_process();
    } else {
      printf("❌ERROR: Comando no válido\n");
    }
    if (input) free_user_input(input);
  }

  cleanup();

  return 0;
}


