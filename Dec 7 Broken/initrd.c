#include "initrd.h"

initrd_header *header;     
initrd_file_header *file_headers; 
fs_node *initrd_root;             
fs_node *initrd_dev;             
fs_node *root_nodes;              
int nroot_nodes;                    

struct dirent dirent;

static u32int initrd_read(fs_node *node, u32int offset, u32int size, u8int *buffer){
   initrd_file_header fheader = file_headers[node->inode];
   if (offset > fheader.length)
       return 0;
   if (offset+size > fheader.length)
       size = fheader.length - offset;
   memcpy(buffer, (u8int*) (fheader.offset+offset), size);
   return size;
}

static u32int initrd_write(fs_node *node, u32int offset, u8int *header, u8int *buffer){
    int sz = 0;
    struct dirent *nodes = 0;
    int i = 0;
    while ((nodes = readdir_fs(initrd_root, i)) != 0){
        if(strcmp(nodes->name, header) == 0){
            fs_node *fsnode = finddir_fs(initrd_root, nodes->name);
            if ((fsnode->flags&0x7) == FS_DIRECTORY){
                monitor_write("target not a file)");
            }else{
                initrd_file_header fheader = file_headers[node->inode];    
                sz = fheader.length; 
                memcpy((u8int*) (fheader.offset+offset), buffer, sz);
                break;
            }
        }
        i++;
    }
    return sz;
}

static struct dirent *initrd_readdir(fs_node *node, u32int index){
   if (node == initrd_root && index == 0){
     strcpy(dirent.name, "dev");
     dirent.name[3] = 0; 
     dirent.inode = 0;
     return &dirent;
   }
   if (index-1 >= nroot_nodes)
       return 0;
   strcpy(dirent.name, root_nodes[index-1].name);
   dirent.name[strlen(root_nodes[index-1].name)] = 0; 
   dirent.inode = root_nodes[index-1].inode;
   return &dirent;
}

static fs_node *initrd_finddir(fs_node *node, char *name){
   if (node == initrd_root &&
       !strcmp(name, "dev") )
       return initrd_dev;
   int i;
   for (i = 0; i < nroot_nodes; i++)
       if (!strcmp(name, root_nodes[i].name))
           return &root_nodes[i];
   return 0;
}

fs_node *init_initrd(u32int location){
   header = (initrd_header *)location;
   file_headers = (initrd_file_header *) (location+sizeof(initrd_header));
   monitor_write("initializing root\n");
   initrd_root = (fs_node*)kmalloc(sizeof(fs_node), 0, 0);
   monitor_write("finished root\n");
   strcpy(initrd_root->name, "initrd");
   initrd_root->mask = initrd_root->uid = initrd_root->gid = initrd_root->inode = initrd_root->length = 0;
   initrd_root->flags = FS_DIRECTORY;
   initrd_root->read = 0;
   initrd_root->write = 0;
   initrd_root->open = 0;
   initrd_root->close = 0;
   initrd_root->readdir = &initrd_readdir;
   initrd_root->finddir = &initrd_finddir;
   initrd_root->ptr = 0;
   initrd_root->impl = 0;
   monitor_write("initializing other directories...\n");
   initrd_dev = (fs_node*)kmalloc(sizeof(fs_node), 0, 0);
   strcpy(initrd_dev->name, "dev");
   initrd_dev->mask = initrd_dev->uid = initrd_dev->gid = initrd_dev->inode = initrd_dev->length = 0;
   initrd_dev->flags = FS_DIRECTORY;
   initrd_dev->read = 0;
   initrd_dev->write = 0;
   initrd_dev->open = 0;
   initrd_dev->close = 0;
   initrd_dev->readdir = &initrd_readdir;
   initrd_dev->finddir = &initrd_finddir;
   initrd_dev->ptr = 0;
   initrd_dev->impl = 0;
   monitor_write("initializing all fs nodes...\n");
   root_nodes = (fs_node*)kmalloc((sizeof(fs_node) * header->nfiles), 0, 0);
   nroot_nodes = header->nfiles;
   int i;
   for (i = 0; i < header->nfiles; i++){
       file_headers[i].offset += location;
       strcpy(root_nodes[i].name, &file_headers[i].name);
       root_nodes[i].mask = root_nodes[i].uid = root_nodes[i].gid = 0;
       root_nodes[i].length = file_headers[i].length;
       root_nodes[i].inode = i;
       root_nodes[i].flags = FS_FILE;
       root_nodes[i].read = &initrd_read;
       root_nodes[i].write = 0;
       root_nodes[i].readdir = 0;
       root_nodes[i].finddir = 0;
       root_nodes[i].open = 0;
       root_nodes[i].close = 0;
       root_nodes[i].impl = 0;
   }
     return initrd_root;
}
