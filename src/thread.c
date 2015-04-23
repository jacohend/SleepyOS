//defines thread creation and switching.
//credit goes to Molloy for the thread pseudo stack idea in create_thread()
//and the thread_exit() error-reporting idea

#include "thread.h"

thread_table *current_thread;
u32int next_tid = 0;

static void thread_exit ();

thread_table *init_threading (){
    thread_table *thread = (thread_table*) kmalloc (sizeof (thread_table*), 0, 0);
    thread->id  = next_tid++;
    current_thread = thread;
    return thread;
}

//place function data fn and arguments arg into pseudostack for 
//context-switching purposes.
thread_table *create_thread (int (*fn)(void*), void *arg, u32int *stack){
    thread_table *thread = (thread_table*)kmalloc (sizeof (thread_table*), 0, 0);
    memset (thread, 0, sizeof (thread_table));
    thread->id = next_tid++;
    *--stack = (u32int)arg;
    *--stack = (u32int)&thread_exit; 
    *--stack = (u32int)fn;
    *--stack = 0; 
    thread->ebp = (u32int)stack;
    thread->esp = (u32int)stack;
    return thread;
}

/*grab respectively the stack pointer, base pointer, frame pointer, 
 *source index pointer, and destination index pointer into memory, 
 *thus saving state. Then move the stack of the next thread into
 *these registers, switching contexts. */
void switch_thread (thread_table *next){
    asm volatile ("mov %%esp, %0" : "=r" (current_thread->esp));
    asm volatile ("mov %%ebp, %0" : "=r" (current_thread->ebp));
    asm volatile ("mov %%ebx, %0" : "=r" (current_thread->ebx));
    asm volatile ("mov %%esi, %0" : "=r" (current_thread->esi));
    asm volatile ("mov %%edi, %0" : "=r" (current_thread->edi));
    current_thread = next;
    asm volatile ("mov %0, %%edi" : : "r" (next->edi));
    asm volatile ("mov %0, %%esi" : : "r" (next->esi));
    asm volatile ("mov %0, %%ebx" : : "r" (next->ebx));
    asm volatile ("mov %0, %%esp" : : "r" (next->esp));
    asm volatile ("mov %0, %%ebp" : : "r" (next->ebp));
}


void thread_exit (){
    register u32int val asm ("eax");
    monitor_write("Thread exited with value ");
    monitor_write_hex(val);
    for (;;) ;
}
