

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "kb.h"

int main(struct multiboot *mboot_ptr){
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();
    // Write out a sample string
    monitor_write("Welcome to SleepyOS\n");
    asm volatile("sti");
    //init_timer(1193180); 
    init_typing();
    return 0;
}
