bits 32 
global start 

extern exit, printf, fopen, fread, fclose
import exit msvcrt.dll
import printf msvcrt.dll
import fopen msvcrt.dll
import fread msvcrt.dll
import fclose msvcrt.dll

segment data use32 class=data
   
    
   
segment code use32 class=code
start:

    mov ah, 128|2; ah = 130 = 1000 0010 = 82h
    mov bh, 90h >> 3; bh =    1001 0000 >> 3 = 0001 0010 = 12h
    sub ah, bh ; ah-bh = 1111 0000 CF = 1, Of = 0, SF = 1, ZF = 0
    
    
    
push dword 0 ; saves on stack the parameter of the function exit
call [exit] ; function exit is called in order to end the execution of the program 