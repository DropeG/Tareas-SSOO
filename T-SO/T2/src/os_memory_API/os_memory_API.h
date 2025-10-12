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

typedef struct {
    uint16_t pid;
    uint16_t file_idx;
    uint32_t vaddr;
    uint64_t size;
    char mode;
} osmFile;
//--------------------------------CHATGPT-------------------------------


/* ====== FUNCIONES GENERALES ====== */

// void mount_memory(char* memory_path);

// void list_processes();

// int processes_slots();

// void list_files(int process_id);

// void frame_bitmap_status();



/* ====== FUNCIONES PARA PROCESOS ====== */

// int start_process(int process_id, char* process_name);

// int finish_process(int process_id);

// int clear_all_processes();

// int file_table_slots(int process_id);


/* ====== FUNCIONES PARA ARCHIVOS ====== */

// osmFile* open_file(int process_id, char* file_name, char mode);

// int read_file(osmFile* file desc, char* dest);

// int write_file(osmFile* file desc, char* src);

// void delete_file(int process id, char* file name);

// void close_file(osmFile* file_desc);


/*====== BONUS =====*/

// int format_memory(char* memory path);
