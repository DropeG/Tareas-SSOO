#pragma once
#include "../osm_File/osm_File.h"


//--------------------------------CHATGPT-------------------------------
#define PCBS_SZ        (8 * 1024)               // 8192
#define IPT_SZ         (192 * 1024)             // 196608
#define BITMAP_SZ      (8 * 1024)               // 8192
#define DATA_SZ        (2ULL * 1024 * 1024 * 1024) // 2GB
#define OFF_PCB        0
#define OFF_IPT        (OFF_PCB + PCBS_SZ) // 8192
#define OFF_BITMAP     (OFF_IPT + IPT_SZ)  // 204800
#define OFF_DATA       (OFF_BITMAP + BITMAP_SZ) // 212992

#define FRAME_SZ       (32 * 1024)        // 32768 (2^15)
#define NUM_FRAMES     (1 << 16)          // 65536



// Variables globales
extern char* memory_path;
extern FILE* mem_file;

extern uint8_t pcb_table[PCBS_SZ];
extern uint8_t ipt[IPT_SZ];
extern uint8_t frame_bitmap[BITMAP_SZ];

//--------------------------------CHATGPT-------------------------------


//------------------Funciones Generales------------------
// void mount_memory(char* memory_path);
void mount_memory(char* path);

// void list_processes();
void list_processes(void);

// int processes_slots();
int processes_slots(void);

// void list_files(int process_id);
void list_files(int process_id);

// void frame_bitmap_status();
void frame_bitmap_status(void);



//------------------Funciones para procesos------------------
// int start_process(int process_id, char* process_name);
int start_process(int process_id, char* process_name);

// int finish_process(int process_id);
int finish_process(int process_id);

// int clear_all_processes();
int clear_all_processes(void);

// int file_table_slots(int process_id);
int file_table_slots(int process_id);


//------------------Funciones para archivos------------------
// osmFile* open_file(int process_id, char* file_name, char mode);
osmFile* open_file(int process_id, char* file_name, char mode);

// int read_file(osmFile* file desc, char* dest);
int read_file(osmFile* file_desc, char* dest);

// int write_file(osmFile* file desc, char* src);
int write_file(osmFile* file_desc, char* src);

// void delete_file(int process id, char* file name);
void delete_file(int process_id, char* file_name);

// void close_file(osmFile* file_desc);
void close_file(osmFile* file_desc);


//------------------Bonus------------------

// int format_memory(char* memory path);
int format_memory(char* memory_path);
