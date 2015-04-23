//Jacob Henderson
//main.c contains kernel and simple shell

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "kb.h"
#include "paging.h"
#include "multiboot.h"
#include "fs.h"
#include "initrd.h"
#include "task_scheduler.h"

//called by threading demo which is found in interpret()
int fn(void *arg){
   monitor_put('\n');
}

//character buffers and shell flags
char buf[100];
char internal_buf[1024];
int input = 0;
int char_ptr;

//filesystem pointer
extern u32int mem_ptr;

//kernel
int main(struct multiboot *mboot_ptr){
    init_descriptor_tables();   //initialize memory segmentation and interrupt tables
    monitor_clear();    //clear monitor
    asm volatile("sti");    //enable interrupts
    init_timer(50);
    monitor_write("initialized interrupts\n");
    ASSERT(mboot_ptr->mods_count > 0);  //see if there's a filesystem module
    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);  //point to start of filesystem module
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);     //point to end of filesystem module 
    mem_ptr = initrd_end;   //set memory pointer to end of filesystem so we know where to start paging and heap
    monitor_write("initializing paging...\n");  
    initialize_paging();    //initialize paging memory management
    monitor_write("page mapping complete\n");
    //initialise_tasking();
    monitor_write("initializing filesystem...\n");
    root = init_initrd(initrd_location);    //initialize filesystem and give kernel the root directory node
    monitor_write("filesystem initialized\n");
    monitor_clear();    //clear monitor
    monitor_write("Welcome to SleepyOS\n");
    init_typing();  //initialize basic shell by registering typing() interrupt handler
    return 0;
}


//most of this below relates to the basic shell
void init_buf(char *cursor){
    monitor_write(cursor);
    int i = 0;
    char_ptr = 0;
    for(i=0;i<100;i++){
        buf[i] = '0';
    }
}


//called by interpret(), views files
void cat (char *file){
    int i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(root, i)) != 0){
        if(strcmp(node->name, file) == 0){
            fs_node *fsnode = finddir_fs(root, node->name);
            if ((fsnode->flags&0x7) == FS_DIRECTORY){
                monitor_write("target not a file)");
            }else{
                char buf[256];
                u32int sz = read_fs(fsnode, 0, 256, buf);
                int j;
                for (j = 0; j < sz; j++){
                    if(buf[j] == '\0')
                        break;
                    monitor_put(buf[j]);
                }
                monitor_write("\n");
            }
        }
        i++;
    }
}


//lists files
void list (){
    int i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(root, i)) != 0){
        if (strlen(node->name) != 0){
            monitor_write(node->name);
            fs_node *fsnode = finddir_fs(root, node->name);

            if ((fsnode->flags&0x7) == FS_DIRECTORY){
                monitor_write(" (dir)");
            }
            monitor_write("\n");
        }
        i++;
    }
}


//writes to a pre-existing file.
//new file functionality not yet added to fc.h or initrd.h
void write (char *file, char*buf){
    int i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(root, i)) != 0){
        if(strcmp(node->name, file) == 0){
            fs_node *fsnode = finddir_fs(root, node->name);
            if ((fsnode->flags&0x7) == FS_DIRECTORY){
                monitor_write("target not a file)");
            }else{
                monitor_write("file found\n");
                u32int sz = write_fs(fsnode, 0, 0, buf);
                    
            }
        }
        i++;
    }
}


//clears monitor
void clear(){
    monitor_clear();
}


//shell
void interpret(){
    int i;
    if (!input){
        for(i=0;i<1024;i++){
            internal_buf[i] = '0';
        }
    }
    if (strcmp("help\0", buf) == 0){
        monitor_write("commands: ls, clear, cat [file], write [to file], threadingdemo\n");
    }
    if(strcmp("threadingdemo\0", buf) == 0){
        monitor_write("This will call 2 threads, each printing a newline\n");
        u32int *stack = kmalloc (0x200, 0, 0);
        thread_table *thread = create_thread(&fn, (void*)0x41, stack);
        add_thread(thread);
        add_thread(thread);
    }

    if (strcmp("timer\0", buf) == 0){
        monitor_write("Initializing 1 second timer...\n");
        monitor_write("Actual times may vary due to system clock variations\n");
        init_timer(1193180); 
    }
    if (strcmp("ls\0", buf) == 0){
        list();
    }
    if (strcmp("clear\0", buf) == 0){
        clear();
    }
    if (strncmp("cat", buf, 3) == 0){
        strtail(buf, internal_buf, 4);
        cat(internal_buf);
    }
    if ((strncmp("write", buf, 5) == 0) || (input)){
        if (!(input)){
            strtail(buf, internal_buf, 4);
            input = 1;
            init_buf("> ");
        } else{
            input = 0;
            write(internal_buf, buf);
        }
    }
    if (!(input))
        init_buf("$ ");
}


//interrupt handler for typing. fills global buffer with input
void typing(registers_t reg){
    unsigned char k;
    char c;
    k = inb(0x60);
    c = key_lookup(k);
    if (k & 0x80){ 
    }
    else{
        if (c == '\n'){
            monitor_put('\n');
            buf[char_ptr++] = '\0';
            interpret();
        }
        else{
            buf[char_ptr++] = c;
            monitor_put(c);
        }
    }
}


//creates typing interrupt
void init_typing(){
    init_buf("$ ");
    register_interrupt_handler(IRQ1, &typing);
}


