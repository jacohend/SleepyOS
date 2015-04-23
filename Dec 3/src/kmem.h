#ifndef KMEM_H
#define KMEM_H

#include "library.h"


u32int kmalloc(u32int size, int page_align, u32int *phys);


#endif
