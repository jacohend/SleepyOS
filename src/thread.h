#ifndef THREAD_H
#define THREAD_H

#include "library.h"

//defines what a thread is
typedef struct{
  u32int id;                  
  u32int esp, ebp, ebx, esi, edi; 
} thread_table;

thread_table *init_threading ();

thread_table *create_thread (int (*fn)(void*), void *arg, u32int *stack);

void switch_thread (thread_table *next);

#endif
