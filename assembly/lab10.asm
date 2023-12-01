%macro output 2
mov eax, 4  
mov ebx, 1  
mov ecx, %1 
mov edx, %2 
int 0x80    
%endmacro

section .text
 
global _start
 
_start:
    
    mov eax, [xMin] ; [xMin;xMax]
    mov [x], eax ; current x = xMin
    
    Expression: ; starting calculate an expression ln(sqrt(1+x^2)/sqrt(1-x^2))
    finit ; math coprocessor
    fld DWORD [x] ; push current x to stek
    
    mov esi, 1 ; esi = 1 for output the x argument
    call A ; call procedure to output the value
    
    output space, spaceLen ; output space
    
    finit ; expression
    fld DWORD [x] ; x
    fld DWORD [x] ; x
    fmul ; x*x
    
    fld1 ; 1
    fadd ; 1+x^2
    
    fsqrt ; sqrt(1+x^2)
    
    fld DWORD [x] ; x
    fld DWORD [x] ; x 
    fmul ; x*x
    
    fld1 ; 1
    fsubr ; 1-x^2
    
    fsqrt ; sqrt(1-x^2)
    
    fdiv ; sqrt(1+x^2)/sqrt(1-x^2)
    
    fstp DWORD [currentTmp] ; currentTmp = value
    
    fldln2 
    fld DWORD [currentTmp] ; ln(value)
    fyl2x
    
    mov esi, [signs] ; signs after dot
    call A ; call procedure to output
    
    output nextLine, 1 ; output the next line
    
    finit
    fld DWORD [x] ; x
    fld DWORD [h] ; h
    faddp ; x + h
    
    fst DWORD [x] ; x = x + h
    fld DWORD [h] ; h
    fld DWORD [xMax]
    fadd
    fucomip ; while x < xMax
    jbe exit
    jmp Expression ; loop
    
    exit: ; end 
    mov     eax, 1
    int     0x80
    
    printNum: ; output numbers from stek procedure
    
    mov ecx, 10 ; 10
    xor ebx, ebx 
    xor edx, edx
    toStek: ; push numbers to stek
    div ecx ; /10
    add edx, dword 48 ; +'0' 
    push edx ; push char value to stek
    xor edx, edx
   
    inc ebx ; iterator
    cmp eax, 0 
    jnz toStek
 
    mov [temp], ebx ;temp = count
    
    fromStek:
    pop eax ; pop number from stek
    mov [n], eax ; n = number
 
    output n, 1 ; output number
  
    dec dword [temp] ; dec iterator
    cmp [temp], dword 0  
    jnz fromStek
    
    ret 
    
    printFrac: ; output float part of the value
    
    FloatPart: ; loop
    fld DWORD [currentFrac] ; currentFrac
    fld DWORD [one] ; 0.0001
    faddp ; +
    fmul DWORD [ten] ; *10
    
    fstp DWORD [currentTmp] ; currentTmp = fracPart*10
    
    fld1 ; 1
    fld DWORD [currentTmp] ; currentTmp
    fprem ; /1
    fst DWORD [currentFrac] ; Frac
    fld DWORD [currentTmp] ; Tmp
    
    fsubr ; Tmp - Frac = Int
    
    fistp DWORD [currentInt] ; Int
    
    mov ecx, [currentInt] ; ecx = currentInt
    add ecx, '0'
    mov [currentInt], ecx ; int 
    
    output currentInt, 1 ; output
    
    dec esi ; iterator
    cmp esi, 0
    jne FloatPart
    
    ret
    
    A: ; main procedure for printing
    
    fldz ; 0
    fcomip ; <0
    jbe Skip
    
    output minus, 1 ; output minus
    
    Skip: ; skip minus if positive
    fabs ; module of the value
    fstp DWORD [currentTmp] ; currentTmp
    
    fld1 ; 1
    fld DWORD [currentTmp] ; currentTmp
    fprem ; currentTmp/1
    fst DWORD [currentFrac] ; FracPart
    fld DWORD [currentTmp] ; TmpPart
    
    fsubr ; -
    
    fistp DWORD [currentInt] ; currentInt
    
    mov eax, [currentInt] ; print current integer
    call printNum
    
    output dot, 1 ; output dot
    
    call printFrac ; call print fracPart
    
    ret

section .data
    x dd -0.9 ; current x
    xMin dd -0.9 ; [xMin]
    xMax dd 0.6 ;[xMax]
    h dd 0.1 ; step
 
    one dd 0.000001 ; 0.0001
    ten dd 10.0 ; 10
    currentInt dd 0.0
    currentFrac dd 0.0
    currentTmp dd 0.0
    
    signs dd 5 ; signs after dot
    
    temp dd 0 ; temp for printing numbers
    n dd 0 ; variable for print
    dot db  "."
    minus db "-"
    space db "     " ; space between arguments and functions
    spaceLen equ $ - space
    nextLine db 0xD ; next line
