#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "os_memory_API.h"

char* path;

// funciones generales

void mount_memory(char* path){
    return 0;
}


void list_processes(void){
    return 0;
}


int processes_slots(void){
    return 0;
}


void list_files(int process_id){
    return 0;
}


void frame_bitmap_status(void){
    return 0;
}




// // funciones procesos

int start_process(int process_id, char* process_name){
    return 0;
}


int finish_process(int process_id){
    return 0;
}


int clear_all_processes(void){
    return 0;
}


int file_table_slots(int process_id){
    return 0;
}




// // funciones archivos

osmFile* open_file(int process_id, char* file_name, char mode){
    return 0;
}


int read_file(osmFile* file_desc, char* dest){
    return 0;
}


int write_file(osmFile* file_desc, char* src){
    return 0;
}


void delete_file(int process_id, char* file_name){
    return 0;
}


void close_file(osmFile* file_desc){
    return 0;
}



// // bonus
int format_memory(char* memory_path){
    return 0;
}

