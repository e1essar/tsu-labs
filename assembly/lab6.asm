section .text

global _start

_start:
   
    mov esi, A ; начало массива
    mov edi, 0 ; смещение
    mov ecx, [len] ; длина массива
    mov ebx, min ; минимальный элемент
    
    L: ;определение минимального элемента
        mov eax, [esi + 4 * edi] ;текущий элемент
        cmp eax, ebx ;сравнение с текущим минимальным
        jge newMin ;если меньше текущего минимального
            mov ebx, eax ;переопределение минимума
        newMin:
        inc edi ;смещение
    loop L

    mov [min], ebx ;запись минимального в переменную
    
    mov esi, A ;начало массива
    mov edi, 0 ;смещение
    mov ecx, [len] ;длина массива
    mov ebx, [min] ;минимальный элемент
    
    L2:
        mov eax, [esi + 4 * edi] ;текущий эелмент массива
        sub eax, ebx ;вычитание минимального из текущего элемента
        mov [esi + 4 * edi], eax ;перезапись массива
        inc edi ;счетчик
    loop L2
 
    ;вывод массива
    mov esi, A ;записываем начало массива
    mov edi, 0 ;смещение
    mov ebp, [len] ;длина массива
 
    output:
    xor eax, eax
    xor ebx, ebx
    xor ecx, ecx
    xor edx, edx
 
    mov eax, [esi + 4 * edi] ;текущий элемент массива
 
    mov ecx, 10 
    StekPush: 
    ;делим eax на 10 и переводим остаток в десятичный код
    div ecx
    add edx, dword 48 ; перевод числа в символ
    ;формирование числа в стеке
    push edx
    xor edx, edx
    inc ebx ;кол-во символов для вывода
    cmp eax, 0
    jnz StekPush
    mov [i], ebx ;количество символов
 
    StekPop:
    ;Записываем символ из стека в регистр и выводим на экран, тем самым выводим само число
    pop eax
    mov [N], eax ; Записываем в переменную значение eax
    
    mov eax, 4
    mov ebx, 1  
    mov ecx, N 
    mov edx, 1 
    int 0x80   
    
    dec dword [i] 
    cmp [i], dword 0 ; 
    jnz StekPop
 
    mov eax, 4
    mov ebx, 1
    mov ecx, space
    mov edx, 1
    int 0x80
    
    inc edi ;следующий элемент массива
    dec ebp
    cmp ebp, dword 0
    jne output

    mov eax, 1
    int 0x80

section .data
    A dd 7, 42, -24, 8, -25, 10, 7, 6, -6, -2
    len dd 10
    min dd 0
    N dd 0
    i dd 0
    space dd ' '
