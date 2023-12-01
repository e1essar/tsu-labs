section .text

global _start

_start:

    
    
    mov esi, 0 ; начальное положение столбца
    mov edi, 0 ; начальное положение строки
    
    mov ebx, 0 ; i
    mov ecx, 0 ; j

    L1: ; loop L1
    cmp ebx, [m] ; while i<m
    jge exit
    mov ecx, 0 ; j = 0
    mov esi, 0 ; starting point of matrix
    
    L2:
    cmp ecx, [n] ; while j<n
    jge exitL2
    
    mov eax, matrix[esi+4*edi] ; eax = matrix[i][j]
    mov edx, array[eax*4] ; edx = array[matrix[i][j]]
    
    cmp edx, 0 ; if (array[] = 0)
    jne A
    
    mov edx, [count] ; count++
    inc edx
    mov [count], edx
    mov edx, 1
    mov array[eax*4], edx ; array[] = 1
    A:
    inc ecx ; j++
    add esi, 4 ; next column
    jmp L2
    
    exitL2:
    
    add edi, [n] ;next string
    inc ebx ; i++
    jmp L1
    
    exit:
    
    mov eax, [count] ; output count
    xor ebx, ebx
    xor ecx,ecx
    xor edx,edx
    
    mov ebx, 10;  
 
continueDIV:
 
    div  ebx
    push edx
    inc  ecx
 
    xor edx, edx 
 
    cmp  eax, 0
    jne continueDIV
 
fromStek:
    pop edx;
    mov [c], edx
    add [c], byte 48
    mov [buff], ecx 
 
    mov edx, 1  
    mov ecx, c
    mov ebx, 1
    mov eax, 4
    int 0x80

    mov ecx, [buff]
 
    loop fromStek

    mov     eax, 1
    int     0x80
    
section .data
    array dd 256 dup(0)
    arrayLen dd 256
    matrix dd 0,0,0,0,0,0
           dd 0,0,0,0,0,0
           dd 0,0,0,0,0,0
           dd 0,0,0,0,0,0
           dd 0,0,0,0,0,1
    m dd 5 ;количество строк
    n dd 6 ;количество столбцов
    c dd 0
    buff dd 0
    count dd 0
           
    
   
