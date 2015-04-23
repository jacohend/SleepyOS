#ifndef KMEM_H
#define KMEM_H

#include "library.h"
#include "ordered_array.h"

#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000
#define HEAP_INDEX_SIZE   0x20000
#define HEAP_MAGIC        0x123890AB
#define HEAP_MIN_SIZE     0x70000

typedef struct{
    u32int magic;   
    u8int is_hole;   
    u32int size;    
} header_t;

typedef struct{
    u32int magic;     
    header_t *header; 
} footer_t;

typedef struct{
    ordered_array_t index;
    u32int start_address; 
    u32int end_address;    
    u32int max_address;   
    u8int supervisor;     
    u8int readonly;       
} heap_t;

heap_t *create_heap(u32int start, u32int end, u32int max, u8int supervisor, u8int readonly);

void *alloc(u32int size, u8int page_align, heap_t *heap);

void free(void *p, heap_t *heap);

u32int kmalloc(u32int size, int page_align, u32int *phys);

void kfree(void *p);

#endif
