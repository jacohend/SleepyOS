#ifndef FS_H
#define FS_H

#include "library.h"

//posix-defined file bit flags
#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08 

struct node;

typedef u32int (*read_type_t)(struct node*,u32int,u32int,u8int*);
typedef u32int (*write_type_t)(struct node*,u32int,u32int,u8int*);
typedef void (*open_type_t)(struct node*);
typedef void (*close_type_t)(struct node*);
typedef struct dirent * (*readdir_type_t)(struct node*,u32int);
typedef struct node * (*finddir_type_t)(struct node*,char *name);

//posix specification
typedef struct node{ 
   char name[128];     
   u32int mask;       
   u32int uid;         
   u32int gid;         
   u32int flags;       
   u32int inode;       
   u32int length;      
   u32int impl;        
   read_type_t read;
   write_type_t write;
   open_type_t open;
   close_type_t close;
   readdir_type_t readdir;
   finddir_type_t finddir;
   struct node *ptr;
} fs_node;

struct dirent {
  char name[128]; 
  u32int inode;     
};



extern fs_node *root; 

u32int read_fs(fs_node *node, u32int offset, u32int size, u8int *buffer);
u32int write_fs(fs_node *node, u32int offset, u8int *header, u8int *buffer);
void open_fs(fs_node *node, u8int read, u8int write);
void close_fs(fs_node *node);
struct dirent *readdir_fs(fs_node *node, u32int index);
fs_node *finddir_fs(fs_node *node, char *name);

#endif
