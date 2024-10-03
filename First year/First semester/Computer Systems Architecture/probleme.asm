bits 32 
global  start 

extern  exit

import  exit msvcrt.dll

segment  data use32 class=data

    s1 db 1, 2, 3, 4 ; declare the string of bytes
    ls1 equ $-s1 ; ls1 = 4
    s2 db 5, 6, 7
    ls2 equ $-s2
    D times (ls1+ls2) db 0
   
segment code use32 class=code
start: 
 
    ; 3. Two byte strings S1 and S2 are given. 
    ;   Obtain the string D by concatenating the elements of S1 from the left hand side to the right hand side
    ;    and the elements of S2 from the right hand side to the left hand side.
    ;       Example:
    ;       S1: 1, 2, 3, 4
    ;       S2: 5, 6, 7
    ;       D: 1, 2, 3, 4, 7, 6, 5
    
    mov ECX, ls1
    mov ESI, 0
    mov EDI, 0
    jecxz sfarsit_repeta
    
    repeta:
        mov AL, [s1+ESI]
        mov [D+EDI], AL
        inc ESI
        inc EDI
        loop repeta
        
    sfarsit_repeta:
    
    mov ECX, ls2
    mov ESI, ECX
    dec ESI ; ESI = ls2 - 1
    jecxz sfarsit_repeta2
    
    repeta2:
        mov AL, [s2+ESI]
        mov [D+EDI], AL
        dec ESI
        inc EDI
        loop repeta2
        
    sfarsit_repeta2:
    
    ; 18. Two byte strings A and B are given. 
    ;   Obtain the string R that contains only the odd positive elements of the two strings.
    ;     Example:
    ;     A: 2, 1, 3, -3
    ;     B: 4, 5, -5, 7
    ;     R: 1, 3, 5, 7 
    
    mov ECX, lA
    mov ESI, 0
    mov EDI, 0
    jecxz sfarsit_repeta3
    
    repeta3:
        mov AL, [A+ESI]
        mov BL, AL
        cmp BL, 0
        jl end_if
        and BL, 1b
        cmp BL, 1
        jne end_if
            mov [R+EDI], AL
            inc EDI
        end_if:
        inc ESI
        loop repeta3
        
    sfarsit_repeta3:
    
    mov ECX, lB
    mov ESI, 0
    jecxz sfarsit_repeta4
    
    repeta4:
        mov AL, [B+ESI]
        mov BL, AL
        cmp BL, 0
        jl end_of_if
        and BL, 1b
        cmp BL, 1
        jne end_of_if
        mov [R+EDI], AL
        inc EDI
        end_of_if:
        inc ESI
        loop repeta4
    
    sfarsit_repeta4:
    
	push dword 0 ; saves on stack the parameter of the function exit
	call [exit] ; function exit is called in order to end the execution of the program 






bits 32 
global  start 
extern  exit
import  exit msvcrt.dll
segment  data use32 class=data

    s dd 0702090Ah, 0B0C0304h, 05060108h
    ls equ $ - s
    d times ls db 0
    
segment code use32 class=code
start: 

    ; 3. An array of doublewords, where each doubleword contains 2 values on a word
    ; (unpacked, so each nibble is preceded by a 0) is given. 
    ; Write an asm program to create a new array of bytes which contain those values (packed on a single byte),
    ; arranged in an ascending manner in memory, these being considered signed numbers.
    
    cld ; DF = 0
    mov ESI, s
    mov EDI, 0
    mov ECX, ls
    jecxz sfarsit

    repeta:
        lodsb
        mov BL, AL
        lodsb
        shl AL, 4
        add BL, AL
        mov [d+EDI], BL
        inc EDI
        dec ECX
        loop repeta
        
    verificare: ; Bubble sort
        cmp AL, 1
        je sfarsit_verificare
        
        mov EDI, 0
        mov ECX, (ls / 2)
        dec ECX
        jecxz sfarsit_sortare
        
        sortare:
            mov AL, 1
            mov BL, [d+EDI]
            mov DL, [d+EDI+1]
            cmp BL, DL
            jle ok
                mov AL, BL
                mov BL, DL
                mov DL, AL
                mov [d+EDI], BL
                mov [d+EDI+1], DL
                mov AL, 0
            ok:
            add EDI, 1
            loop sortare
            
        sfarsit_sortare:
        loop verificare
        
    sfarsit_verificare:
    
    sfarsit:
        
    
	push dword 0 ; saves on stack the parameter of the function exit
	call [exit] ; function exit is called in order to end the execution of the program 

asm+asm:

bits 32 
global start 

extern exit, printf, fopen, fread, fclose
import exit msvcrt.dll
import printf msvcrt.dll
import fopen msvcrt.dll
import fread msvcrt.dll
import fclose msvcrt.dll

segment  data use32 class=data
    
    file_name db "numbers.txt", 0
    access_mode db "r", 0
    file_descriptor dd -1
    len equ 100
    numbers_string times len db 0
    format db "%d", 0
   
segment code use32 class=code
start:

    ; 18. Read from file numbers.txt a string of numbers (positive and negative). Build two strings using read numbers:
    ;     P – only with positive numbers
    ;     N – only with negative numbers
    ;     Display the strings on the screen.
    
 
    ; Reading from file
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
    push dword numbers_string        
    call [fread]
    add esp, 4*4

    push dword [file_descriptor]
    call [fclose]
    add esp, 4
    
positive_nrs:
    
        mov esi, 0
        repeta_pozitiv:
        
            mov ebx, 0
            mov bl, [numbers_string + esi]
            
            cmp bl, ' '
            je sfarsit_pozitiv
            
            cmp bl, '-'
            je sfarsit_pozitiv_2
            
            sub bl,'0'
            cmp bl, 0
            jle sfarsit_pozitiv
            
            push ebx
            push dword format
            call [printf]
            add esp, 4*2
            
            sfarsit_pozitiv:
            dec esi
            
            sfarsit_pozitiv_2:
            add esi, 2
            
            cmp ebx, 0
            jne repeta_pozitiv
            
negative_nrs:
    
        mov esi, 0
        repeta_negativ:
        
            mov ebx, 0
            mov bl, [numbers_string + esi]
            
            cmp bl, ' '
            je sfarsit_negativ
            
            cmp bl, '-'
            jne sfarsit_negativ_2
            
            inc esi
            mov bl, [numbers_string + esi]
            sub ebx, '0'
            cmp ebx, 0
            jle sfarsit_negativ
            
            push ebx
            push dword format
            call [printf]
            add esp, 4*2
            
            sfarsit_negativ:
            dec esi
            
            sfarsit_negativ_2:
            add esi, 2
            
            cmp ebx, 0
            jne repeta_negativ
   
   final:
push dword 0 ; saves on stack the parameter of the function exit
call [exit] ; function exit is called in order to end the execution of the program 



EXAMEN:

- scrieti si exemple la subiectul de teorie, chiar daca nu cere ca se puncteaza
- Doamne ajuta!!