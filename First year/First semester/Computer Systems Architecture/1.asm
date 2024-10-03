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
    
    mov ebx, 0
    clc
    repeta:
        shr eax, 1; CF = excluded bit
        adc ebx, 0 ; EBX += CF
        cmp eax, 0
        jne repeta

    push dword ebx
    push dword format_a
    call [printf]
    add esp, 4*2
    
	push dword 0 ; saves on stack the parameter of the function exit
	call [exit] ; function exit is called in order to end the execution of the program 
