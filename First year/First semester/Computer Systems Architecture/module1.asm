bits 32
global extract_numbers

segment code use32 class=code
    extract_numbers:
    
        mov ecx, [esp+4]
        mov esi, 0
        mov bl, [esp+4]
        add bl, [esp+8]
        mov edi, bl
        
        
        repeta:
            mov al, [esi]
            cmp al, '0'
            jl sfarsit
            cmp al, '9'
            jg sfarsit
            movsb
            sfarsit:
            loop repeta
            
        
        mov ecx, [esp+8]
        repeta2:
            mov al, [esi]
            cmp al, '0'
            jl sfarsit2
            cmp al, '9'
            jg sfarsit2
            movsb
            sfarsit2:
            loop repeta2
            
        ret 4*2