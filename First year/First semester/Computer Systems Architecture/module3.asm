bits 32
global negative_nrs

extern printf
import printf msvcrt.dll

segment  data use32 class=data
    
    format db "%d", 0
    
segment code use32 class=code

    negative_nrs:
    
        mov eax, [esp]
        mov esi, 0
        repeta_negativ:
        
            mov ebx, 0
            mov bl, [eax + esi]
            
            cmp bl, ' '
            je sfarsit_negativ
            
            cmp bl, '-'
            jne sfarsit_negativ_2
            
            inc esi
            mov bl, [eax + esi]
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
            
            cmp ebx, 208 ; empty space in memory - D0 in hexa
            jne repeta_negativ
            
        ret 4