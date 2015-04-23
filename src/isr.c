//

#include "library.h"
#include "isr.h"
#include "monitor.h"

//index of interrupts
isr_t interrupt_handlers[256];


//calls interrupt handler if allowed. called by asm interrupt function
void isr_handler(registers_t regs){
    if (interrupt_handlers[regs.int_no] != 0){
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    else{
        monitor_write("unhandled interrupt: ");
        monitor_write_dec(regs.int_no);
        monitor_put('\n');
    }
}

//handles hardware interrupt request
void irq_handler(registers_t regs){
   if (regs.int_no >= 40){
       outb(0xA0, 0x20);
   }
   outb(0x20, 0x20);
   if (interrupt_handlers[regs.int_no] != 0){
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
}

//associate handler function and interrupt number
void register_interrupt_handler(u8int n, isr_t handler){
  interrupt_handlers[n] = handler;
}
