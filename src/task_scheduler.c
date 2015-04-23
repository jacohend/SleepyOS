#include "task_scheduler.h"


thread_stack *stack = 0;
thread_stack *now = 0;

//add thread to scheduler queue
void add_thread(thread_table *thread){
    if (now->thread == 0){
        now = (thread_stack*) kmalloc (sizeof(thread_stack*), 0, 0);
        now->thread = thread;
        now->next = 0;
        return;
    }
    thread_stack *threaded = (thread_stack*) kmalloc (sizeof(thread_stack*), 0, 0);
    threaded->thread = thread;
    threaded->next = 0;
    if(stack ==  0){
        stack = threaded;
    } else {
        thread_stack *temp = stack;
        while(temp->next){
            temp = temp->next;
        }
        temp->next = threaded;
     }
}

//delete thread from queue
void delete_thread(thread_table *thread){
    if (stack == 0)
        return;
    thread_stack *temp = stack;
    if(temp->thread == thread){
        stack = temp->next;
        kfree(temp);
        return;
    }
    while(temp->next){
        if (temp->next->thread == thread){
            thread_stack *temporary = temp->next;
            temp->next = temporary->next;
            kfree(temporary);
        }
        temp = temp->next;
    }
}

//round robin 'block multithreaded' scheduler, iterates through threads
void schedule (){
    if(stack == 0){
        return;
    }
    thread_stack *temp = stack;
    while(temp->next){
        temp = temp->next;
    }
    temp->next = now;
    now = stack;
    stack = stack->next;
    switch_thread(now->thread);
}

