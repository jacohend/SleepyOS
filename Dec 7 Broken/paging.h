#ifndef PAGING_H
#define PAGING_H

#include "library.h"
#include "isr.h"

typedef struct page{
   u32int present    : 1;   
   u32int rw         : 1;   
   u32int user       : 1;   
   u32int accessed   : 1;   
   u32int dirty      : 1;   
   u32int unused     : 7;   
   u32int frame      : 20;  
} page_properties;

typedef struct page_table{
    page_properties pages[1024];
} page_array;

typedef struct page_directory{
   page_array *tables[1024];
   u32int physical_tables[1024];
   u32int physical_address;
} page_directory_table;

void initialize_paging();

void switch_page_directory(page_directory_table *new);

page_properties *get_page(u32int address, int make, page_directory_table *dir);

void page_fault(registers_t regs);

page_directory_table *clone_dir(page_directory_table *original);

#endif
