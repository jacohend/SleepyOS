#include "fs.h"

fs_node *root = 0;
 
u32int read_fs(fs_node *node, u32int offset, u32int size, u8int *buffer){
    if (node->read != 0){
        return node->read(node, offset, size, buffer);
    } 
    return 0;
}

u32int write_fs(fs_node *node, u32int offset, u8int *header, u8int *buffer){
    if (node->write != 0){
        return node->write(node, offset, header, buffer);
    } 
    return 0;
}

void open_fs(fs_node *node, u8int read, u8int write){
    if (node->open != 0){
        return node->open(node);
    } 
}

void close_fs(fs_node *node){
    if (node->read != 0){
        return node->close(node);
    } 
}

struct dirent *readdir_fs(fs_node *node, u32int i){
    if ((node->flags&0x7) == FS_DIRECTORY && node->readdir != 0){
        return node->readdir(node, i);
    }
    return 0;
}

fs_node *finddir_fs(fs_node *node, char *name){
    if ((node->flags&0x7) == FS_DIRECTORY && node->finddir != 0){
        return node->finddir(node, name);
    }
    return 0;
}




