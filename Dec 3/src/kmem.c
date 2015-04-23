#include "kmem.h"

extern u32int end;
u32int mem_ptr = (u32int)&end;

u32int kmalloc(u32int size, int page_align, u32int *phys){
    if (page_align && (mem_ptr & 0xFFFFF000)){
        mem_ptr &= 0xFFFFF000;
        mem_ptr += 0x1000;
    }
    if (phys){
        *phys = mem_ptr;
    }
    u32int result = mem_ptr;
    mem_ptr += size;
    return result;
}



