[GLOBAL copy_page_physical]
copy_page_physical:
    push ebx              
    pushf                                           
    cli                   ;kill interrupts                         
    mov ebx, [esp+12]     
    mov ecx, [esp+16]      
    mov edx, cr0          
    and edx, 0x7fffffff   
    mov cr0, edx          ;kill paging so we don't page fault while copying
    mov edx, 1024         
  
.loop:
    mov eax, [ebx]        ;copy data
    mov [ecx], eax        
    add ebx, 4           
    add ecx, 4            
    dec edx               
    jnz .loop             
    mov edx, cr0          ;reenable paging
    or  edx, 0x80000000   
    mov cr0, edx            
    popf                 
    pop ebx               
    ret


[GLOBAL read_eip]
read_eip:
    pop eax                     
    jmp eax     
