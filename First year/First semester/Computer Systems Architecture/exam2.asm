bits 32 
global start 

extern exit
import exit msvcrt.dll

segment  data use32 class=data
        
    x dw -256, 256h ; 256d = FF00h -> 00 FF, 256h = 0256 -> 56 02 ; $ = 2*2 = 4
    y dw 256 | -256, 256h & 256 ; 256 OR -256 = -256 -> 00 FF, 256h AND 256 = 0000 0010 0101 0110 AND 0000 0001 0000 0000 -> 00 00 ; $ = 4 + 2*2 = 8
    z db $-z, y-x ; $ = z -> 00 04 ; $ = 8 + 1*2 = 10
      db 'y'-'x', 'y-x' ; -> 01 79 2D 78; $ = 12 (y = 121 = 79h, x = 120 = 78h, - = 45 = 2Dh)
    a db 512 >> 2, -512 << 2 ; 512 : 2*2 = 128 -> 80, -512 << 2 = (lb) 0000 0000 -> 00; $ = 14
    b dw z-a, !(z-a)  ; 8 - 14 = -6 = FFFAh -> FA FF, !(z-a) = !(FFFAh) = 0005h -> 05 00 ?? -> 00 00 ; $ = 18
    
    ; c dd ($-b) + (d-$), $-2*y+3 doesn't work
    
    d db -128, 128^(~128) ; -128 -> 80 , 128^(~128) -> FF ; $ = 20
    e times 2 resw 6 ; '00' * 2*6*2 = 24 '00' bytes ; $ = 20 + 24 = 44
    times 2 dd 1234h, 5678h ; -> 34 12 00 00 78 56 00 00 34 12 00 00 78 56 00 00 ; $ = 44 + 2 * 8 = 60
    
segment code use32 class=code
start: 
    
    
push dword 0 ; saves on stack the parameter of the function exit
call [exit] ; function exit is called in order to end the execution of the program 