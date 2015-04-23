#ifndef LIBRARY_H
#define LIBRARY_H

typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

#define ASSERT(b) ((b) ? (void)0 : kassert(__FILE__, __LINE__, #b))


void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

extern void kassert(const char *file, u32int line, const char *desc);


#endif
