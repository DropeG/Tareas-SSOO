#pragma once
#include <stdint.h>

//--------------------------------CHATGPT-------------------------------
typedef struct {
    uint8_t valid;
    char name[15]; // terminado en '\0' en memoria
    // campos en memoria: size (5 bytes), vaddr (4 bytes)
    // en RAM:
    uint64_t size;
    uint32_t vaddr;
} FileTableEntry;

//--------------------------------CHATGPT-------------------------------
