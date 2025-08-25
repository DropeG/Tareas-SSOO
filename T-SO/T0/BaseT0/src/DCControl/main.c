
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

int main(int argc, char const *argv[])
{
  set_buffer(); // No borrar

  while (1) {
    char** input = read_user_input();
    if (string_equals(input[0], "launch")) {
      
    } else if (string_equals(input[0], "status")) {
      printf("COMANDO STATUS\n");
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


