

SOURCES=boot.o main.o monitor.o library.o descriptor_tables.o isr.o interrupts.o timer.o kb.o kmem.o paging.o ordered_array.o fs.o initrd.o process.o thread.o task_scheduler.o

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32
LDFLAGS=-Tlink.ld -melf_i386
ASFLAGS=-felf

all: $(SOURCES) link

clean:
	-rm *.o kernel

link:
	ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
