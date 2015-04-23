#ifndef INITRD_H
#define INITRD_H

#include "library.h"
#include "kmem.h"
#include "fs.h"

typedef struct{
   u32int nfiles; 
} initrd_header;

typedef struct{
   u8int magic;     
   s8int name[64];  
   u32int offset;   
   u32int length;   
   } initrd_file_header;

fs_node *init_initrd(u32int location);

#endif
