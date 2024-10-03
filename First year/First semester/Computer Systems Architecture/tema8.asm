bits 32 
global  start 

extern  exit, printf
import  exit msvcrt.dll
import  printf msvcrt.dll

segment  data use32 class=data
    a dd -3
    b dd 2
    result_format db "%d + %d = %d", 10, 0  ; Format string for printf
   
segment code use32 class=code
start: 

    ; 3. Two natural numbers a and b (a, b: dword, defined in the data segment) are given. 
    ;    Calculate their sum and display the result in the following format: "<a> + <b> = <result>". 
    ;    Example: "1 + 2 = 3"
    ;    The values will be displayed in decimal format (base 10) with sign.
    
    ; Load values of a and b into registers
    mov eax, [a]
    mov ebx, [b]

    ; Add a and b
    add eax, ebx
    adc eax, 0
    
    push eax
    push dword [b]
    push dword [a]
    push result_format
    call [printf]
    add esp, 4*4
    
push dword 0 ; saves on stack the parameter of the function exit
call [exit] ; function exit is called in order to end the execution of the program 



bits 32 
global  start 

extern  exit, scanf, printf
import  exit msvcrt.dll
import  printf msvcrt.dll
import  scanf msvcrt.dll

segment  data use32 class=data

    a dd 0
    format_a db "%d", 0
    b dd 0
    format_b db "%h", 0
    
segment code use32 class=code
start: 

    ; 18. Read a decimal number and a hexadecimal number from the keyboard. 
    ;     Display the number of 1's of the sum of the two numbers in decimal format. Example:
    ;     a = 32 = 0010 0000b
    ;     b = 1Ah = 0001 1010b
    ;     32 + 1Ah = 0011 1010b
    ;     The value printed on the screen will be 4
    
    push dword a
    push dword format_a
    call [scanf]
    add esp, 4*2
    
    push dword b
    push dword format_b
    call [scanf]
    add esp, 4*2
    

    mov eax, [a]
    add eax, [b]
    adc eax, 0
    
    mov ebx, 0
    clc
    repeta:
        shl eax, 1; CF = excluded bit
        adc ebx, 0 ; EBX += CF
        cmp eax, 0
        jne repeta

    push dword ebx
    push dword format_a
    call [printf]
    add esp, 4*2
    
	push dword 0 ; saves on stack the parameter of the function exit
	call [exit] ; function exit is called in order to end the execution of the program 



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