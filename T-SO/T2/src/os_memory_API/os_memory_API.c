#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "os_memory_API.h"

char* path;

// funciones generales

void mount_memory(char* path){
    // Guardar ruta global
    memory_path = strdup(path);

    // Abrir archivo binario en modo lectura/escritura
    mem_file = fopen(memory_path, "rb+");
    if (!mem_file){
        perror("Error al abrir memoria");
        exit(1);
    }

    // Leer secciones en RAM
    fseek(mem_file, OFF_PCB, SEEK_SET);
    fread(pcb_table, 1, PCBS_SZ, mem_file);

    fseek(mem_file, OFF_IPT, SEEK_SET);
    fread(ipt, 1, IPT_SZ, mem_file);

    fseek(mem_file, OFF_BITMAP, SEEK_SET);
    fread(frame_bitmap, 1, BITMAP_SZ, mem_file);
    return;
}


void list_processes(){
    for (int i = 0; i < 32; i++){
        uint8_t estado = pcb_table[i * 256];
        if (estado == 0x01){
            char name[15];
            memcpy(name, &pcb_table[i * 256 + 1], 14);
            name[14] = '\0';
            uint8_t pid = pcb_table[i * 256 + 15];
            printf("%d %s\n", pid, name);
        }
    }
}


int processes_slots(){
        int free = 0;
    for (int i = 0; i < 32; i++){
        uint8_t estado = pcb_table[i * 256];
        if (estado == 0x00){
            free++;
        }
    }
    return free;
}


void list_files(int process_id){
    return;
}


void frame_bitmap_status(){
    int usados = 0;
    for (int i = 0; i < NUM_FRAMES; i++){
        int byte = i / 8;
        int bit = i % 8;
        if (frame_bitmap[byte] & (1 << bit)){
            usados++;
        }
    }
    printf("USADOS: %d LIBRES: %d\n", usados, NUM_FRAMES - usados);
}




// // funciones procesos

int start_process(int process_id, char* process_name){
    return 0;
}


int finish_process(int process_id){
    return 0;
}


int clear_all_processes(void){
    int count = 0;
    for (int i = 0; i < 32; i++){
        if (pcb_table[i*256] == 0x01){
            int pid = pcb_table[i*256+15];
            if (finish_process(pid) == 0){
                count++;
            }
        }
    }
    return count;
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
    return;
}


void close_file(osmFile* file_desc){
    if (file_desc){
        free(file_desc);
    }
}


// // bonus
int format_memory(char* memory_path){
    return 0;
}

