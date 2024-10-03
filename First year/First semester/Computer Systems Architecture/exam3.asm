bits 32 
global start 

extern exit
import exit msvcrt.dll

segment  data use32 class=data

    ; ASC EXAM 18.02.2019  2. a.
        
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



    ; ASC EXAM 18.02.2019  2. b.

    mov bh, 7Fh ; bl (rapid conv.) = 0111 1111b
    cmp bh, al ; al = val -> bh < al ? CF = 1 : CF = 0
    rcr ah, 1 ; rotate w/ carry right -> ah = 0/1xxx xxxx ; CF = 0/1
    sar ah, 7 ; shift arithmetic RIGHT - filling new vacant spots WITH THE SIGN BIT (leftmost initial bit), last disappearing bit stored in CF
    
    ; ex:
    ; al = 0xxx xxxx -> bh = 0111 1111 > 0xxx xxxx -> CF = 0
    ; -> al = 1xxx xxxx -> bh = 0111 1111 < 1xxx xxxx -> CF = 1
    ; -> ah = 0/1xxx xxxx
    ; -> ah = 0000 0000 / 1111 1111
    
    ; ==> equivalent with cbw !!


    ; ASC EXAM 18.02.2019 3. a.

    mov eax, 200 ; 128 + 64 + 8 = 200 = ax = 0000 0000 1100 1000
    mov ebx, 254h ; bl = 0101 0100b = 4 + 16 + 64 = 84
    idiv bl ; ax : bl = 200 : 84 -> ah = 32 , al = 2

    mov ah, 129; ah = 1000 0001 = 81h
    mov bh, 9Fh; bh = 1001 1111
    add ah, bh ; ah+bh = 1 0010 0000 -> CF = 1, OF = 1, ZF = 0, SF = 0

    mov ax, 128 ; ah = 00h = 0
    sar al, 7 ; al = FFh = -1
    imul ah ; ax = ah * al = 00 ; CF = OF = 0, ZF = 0, SF = 1 ???

    mov ax, 256 ; ah = 01h
    mov bx, -1 ; bh = FFh
    add ah, bh ; ah+bh = 00h CF = 1, OF = 0, ZF = 1, SF = 0

    mov ah, 128|2; ah =  1000 0010 = 82h
    mov bh, 90h >> 3; bh=0001 0010 = 12h
    sub ah, bh ; ah-bh = 0111 0000 CF = 0, OF = 1, SF = 0, ZF = 0





    

    ; ASC EXAM 23.01.2019 2.

    segment  data use32 class=data
   
        flags dd 0h12345678, 0x34567812, 56781234h, 78123456h
        v db 0
   
    segment code use32 class=code
    start:

    mov ecx, 2
    mov al, [flags + ecx + 1] ; al = 12
    mov bl, [flags + ecx * 2 - 1] ; bl = 12
    mov bl, [flags + ecx] ; bl = 34
    mov bl, [flags] ; bl = 78