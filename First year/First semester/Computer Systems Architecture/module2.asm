bits 32
global positive_nrs

extern printf
import printf msvcrt.dll

segment data use32 class=data
    
    format db "%d", 0
    
segment code use32 class=code

    positive_nrs:
    
        mov eax, [esp]
        mov esi, 0
        repeta_pozitiv:
        
            mov ebx, 0
            mov bl, [eax + esi]
            
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
            
            cmp ebx, 208 ; empty space in memory - D0 in hexa
            jne repeta_pozitiv
            
            ret 4
    