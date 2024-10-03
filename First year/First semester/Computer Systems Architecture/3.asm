bits 32 
global  start 

extern exit, fopen, fread, fclose, printf
import exit msvcrt.dll
import fopen msvcrt.dll
import fread msvcrt.dll
import fclose msvcrt.dll
import printf msvcrt.dll

segment  data use32 class=data
    
    file_name db "tema8pb18.txt", 0
    access_mode db "r", 0
    file_descriptor dd -1
    len equ 100
    text times len db 0
    dec_format db "%d", 0
   
segment code use32 class=code
start: 

    ; 18. A text file is given. 
    ; The text contains letters, spaces and points. 
    ; Read the content of the file, determine the number of words and display the result on the screen. 
    ; (A word is a sequence of characters separated by space or point)
    
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
    mov esi, 0
    mov ebx, 0
    jecxz finished_repeta
    repeta:
        mov al, [text+esi]
        
        cmp al, ' '
        je word_found
        cmp al, '.'
        je word_found
        
        cmp al, 'A'
        jl word_not_found
        cmp al, 'Z'
        jg word_not_found
        cmp al, 'a'
        jl word_not_found
        cmp al, 'z'
        jg word_not_found
        
        word_found:
        add ebx, 1
        
        word_not_found:
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