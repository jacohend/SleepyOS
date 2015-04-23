//Jacob Henderson

#include "library.h"

//Functions to read/write a byte in/out to/from the specified port. 
//From OSdev wiki
void outb(u16int port, u8int value){
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port){
    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

u16int inw(u16int port){
    u16int ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

//copy data from one location to another
void memcpy(u8int *dest, const u8int *src, u32int len){
    const u8int *sp = (const u8int *)src;
    u8int *dp = (u8int *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

//set memory to value
void memset(u8int *dest, u8int val, u32int len){
    u8int *temp = (u8int *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}


//string handling from here on down. 
int strcmp(char *str1, char *str2){
      int i = 0;
      int failed = 0;
      while(str1[i] != '\0' && str2[i] != '\0'){
          if(str1[i] != str2[i]){
              failed = 1;
              break;
          }
          i++;
      } 
      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
      return failed;
}


 int strncmp(const char *s1, const char *s2, int n){
     unsigned char uc1, uc2;
     if (n == 0)
         return 0;
     while (n-- > 0 && *s1 == *s2) {
         if (n == 0 || *s1 == '\0')
             return 0;
         s1++;
         s2++;
     }
     uc1 = (*(unsigned char *) s1);
     uc2 = (*(unsigned char *) s2);
     return ((uc1 < uc2) ? -1 : (uc1 > uc2));
 }


void *strtail(char *str, char *dest){
    while (*str != ' '){
        str++;
    }
    str++;
     do{
      *dest++ = *str++;
    }
    while (*str != '0');
}


char *strcpy(char *dest, const char *src){
    do{
      *dest++ = *src++;
    }
    while (*src != 0);
}

char *strcat(char *dest, const char *src){
    while (*dest != 0){
        *dest = *dest++;
    }
    do{
        *dest++ = *src++;
    }
    while (*src != 0);
    return dest;
}

int strlen(char *src){
    int i = 0;
    while (*src++)
        i++;
    return i;
}

//much-needed assert for error checking
extern void kassert(const char *file, u32int line, const char *desc){
    asm volatile("cli"); 
    monitor_write("ASSERTION-FAILED(");
    monitor_write(desc);
    monitor_write(") at ");
    monitor_write(file);
    monitor_write(":");
    monitor_write_dec(line);
    monitor_write("\n");
    for(;;);
}
