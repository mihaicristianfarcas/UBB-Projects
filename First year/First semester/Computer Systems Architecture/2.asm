bits 32 
global  start 

extern exit, fopen, fread, fclose, printf
import exit msvcrt.dll
import fopen msvcrt.dll
import fread msvcrt.dll
import fclose msvcrt.dll
import printf msvcrt.dll

segment  data use32 class=data
    
    file_name db "tema8pb3.txt", 0
    access_mode db "r", 0
    file_descriptor dd -1
    len equ 20
    text times len db 0
    dec_format db "%d", 0
   
segment code use32 class=code
start: 

    ;3. A text file is given. 
    ;   Read the content of the file, count the number of even digits and display the result on the screen. 
    ;   The name of text file is defined in the data segment.
    
    push dword access_mode     
    push dword file_name
    call [fopen]
    add esp, 4*2

    mov [file_descriptor], eax
    cmp eax, 0
    je final

    push dword [file_descriptor]
    push dword len
    push dword 1
    push dword text        
    call [fread]
    add esp, 4*4

    
    mov ecx, eax
    mov eax, 0
    mov esi, 0
    mov ebx, 0
    jecxz finished_repeta
    repeta:
        mov al, [text+esi]
        cmp al, '0'
        jl pass
        cmp al, '9'
        jg pass
        sub al, '0'
        and al, 1
        cmp al, 0
        jne pass
        add ebx, 1
        pass:
        inc esi
        loop repeta
        
    finished_repeta:
    push dword ebx
    push dword dec_format
    call [printf]
    add esp, 4*2
    
    push dword [file_descriptor]
    call [fclose]
    add esp, 4
    
    final: 
push dword 0 ; saves on stack the parameter of the function exit
call [exit] ; function exit is called in order to end the execution of the program 

