#include "kb.h"
#include "isr.h"
#include "timer.h"
#include "monitor.h"

char buf[100];
int char_ptr = 0;

/* standard US keyboard array from OSdev */
unsigned char kbarray[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};	


void keyboard_handler(registers_t regs){
    unsigned char scancode;

    scancode = inb(0x60);

    if (scancode & 0x80){
    }
    else{
         monitor_put(kbarray[scancode]);
    }
}

void init_buf(){
    monitor_put('>');
    char_ptr = 0;
    int i = 0;
    for(i=0;i<100;i++){
        buf[i] = '0';
    }
}

void interpret(){
    if (strcmp("help\0", buf) == 0){
        monitor_write("We only support the 'timer' command right now\n");
    }
    if (strcmp("timer\0", buf) == 0){
        monitor_write("Initializing 1 second timer...\n");
        monitor_write("Actual times may vary due to system clock variations\n");
        init_timer(1193180); 
    }
    init_buf();
}

void typing(registers_t reg){
    unsigned char k;
    char c;
    k = inb(0x60);
    c = kbarray[k];
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
    init_buf();
    register_interrupt_handler(IRQ1, &typing);
}

void init_keyboard(){

    register_interrupt_handler(IRQ1, &keyboard_handler);

}


