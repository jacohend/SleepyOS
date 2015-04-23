#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include "library.h"
#include "kmem.h"
#include "thread.h"

typedef struct thread_node{
    thread_table *thread;
    struct thread_node *next;
} thread_stack;

void add_thread(thread_table *thread);

void delete_thread(thread_table *thread);

void schedule();

#endif

