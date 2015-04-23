#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "kb.h"
#include "paging.h"
#include "multiboot.h"
#include "fs.h"
#include "initrd.h"


char buf[100];
char internal_buf[1024];
int input = 0;
int char_ptr;
extern u32int mem_ptr;
void list ();


int main(struct multiboot *mboot_ptr){
    init_descriptor_tables();
    monitor_clear();
    asm volatile("sti");
    monitor_write("initialized interrupts\n");
    ASSERT(mboot_ptr->mods_count > 0);
    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    mem_ptr = initrd_end;
    monitor_write("initializing paging...\n");
    initialize_paging();
    monitor_write("page mapping complete\n");
    monitor_write("initializing filesystem...\n");
    root = init_initrd(initrd_location);
    monitor_write("filesystem initialized\n");
    monitor_clear();
    monitor_write("Welcome to SleepyOS\n");
    init_typing();
    return 0;
}


void init_buf(char *cursor){
    monitor_write(cursor);
    int i = 0;
    char_ptr = 0;
    for(i=0;i<100;i++){
        buf[i] = '0';
    }
}


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
                for (j = 0; j < sz; j++)
                    monitor_put(buf[j]);
                monitor_write("\"\n");
            }
        }
        i++;
    }
}

void run (char *file){
    int i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(root, i)) != 0){
        if(strcmp(node->name, file) == 0){
            fs_node *fsnode = finddir_fs(root, node->name);
            if ((fsnode->flags&0x7) == FS_DIRECTORY){
                monitor_write("target not a file)");
            }else{
                unsigned char buf[1500];
                u32int sz = read_fs(fsnode, 0, 1500, buf);
                int j = 0;
                for (j = 0; j < sz; j++){
                    monitor_write_hex(buf[j]);
                }
                monitor_write("\"\n");      
            }
        }
        i++;
    }
}


void clear(){
    monitor_clear();
}


void interpret(){
    int i;
    if (!input){
        for(i=0;i<1024;i++){
            internal_buf[i] = '0';
        }
    }
    if (strcmp("help\0", buf) == 0){
    monitor_write("try \n");
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
            init_buf(">");
        } else{
            input = 0;
            write_fs(root, 0, internal_buf, buf);
       }
    }
    init_buf("$ ");
}


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

void init_typing(){
    init_buf("$ ");
    register_interrupt_handler(IRQ1, &typing);
}



