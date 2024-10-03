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

