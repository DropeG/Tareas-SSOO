#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//--------------------------------CHATGPT-------------------------------
typedef struct {
    uint8_t valid;
    char name[15]; // terminado en '\0' en memoria
    // campos en memoria: size (5 bytes), vaddr (4 bytes)
    // en RAM:
    uint64_t size;
    uint32_t vaddr;
} FileTableEntry;


typedef struct {
    uint16_t pid;
    uint16_t file_idx;
    uint32_t vaddr;
    uint64_t size;
    char mode;
} osmFile;


osmFile* open_file(uint8_t process_id, char* file_name, char mode);
int read_file(osmFile* file_desc, char* dest);
int write_file(osmFile* file_desc, char* src);
void delete_file(uint8_t process_id, char* file_name);
void close_file(osmFile* file_desc);


//--------------------------------CHATGPT-------------------------------
