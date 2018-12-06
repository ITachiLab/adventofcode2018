section .data

sum:        dd  0
input:      db  "input.txt",0
crlf:       db  $0d,$0a

section .bss

read_buffer     resb 8
outbuf          resb 1

section .text

global _start

; ebp - 4 - file descriptor
; ebp - 8 - just 10
; ebp - 12 - counter
_start:
    push    ebp
    mov     ebp, esp
    sub     esp, 12

    mov     dword [ebp-8], 10
    mov     dword [ebp-12], 0
    
    mov     eax, 5
    mov     ebx, input
    xor     ecx, ecx
    mov     edx, 4
    int     80h

    mov     [ebp-4], eax

_repeat_reading:
    push    dword [ebp-4]
    call    _read_line
    add     esp, 4

    test    eax, eax
    jz      _close

    push    eax
    call    _parse_line
    add     esp, 4

    jmp     _repeat_reading

_close:
    mov     ebx, [ebp-4]
    mov     eax, 6
    int     80h

    cmp     dword [sum], 0
    jge     _non_neg

    mov     byte [outbuf], '-'
    mov     eax, 4
    mov     ebx, 1
    mov     edx, 1
    mov     ecx, outbuf
    int     80h

    mov     eax, dword [sum]
    neg     eax
    mov     dword [sum], eax

_non_neg:
    mov     eax, dword [sum]
    cdq

_div:
    xor     edx, edx
    div     dword [ebp-8]

    add     edx, 30h
    push    edx
    inc     dword [ebp-12]

    test    eax, eax
    jnz     _div

_print:
    cmp     dword [ebp-12], 0
    je      _exit

    mov     eax, 4
    mov     ebx, 1
    mov     edx, 1
    mov     ecx, esp
    int     80h

    add     esp, 4
    dec     dword [ebp-12]

    jmp     _print

_exit:
    mov     eax, 4
    mov     ebx, 1
    mov     edx, 2
    mov     ecx, crlf
    int     80h

    mov     esp, ebp
    pop     ebp

    mov     eax, 1
    mov     ebx, 0
    int     80h

; ebp + 8 - file
; ebp - 4 - read character
; ebp - 8 - character count
;
; return - number of read characters
_read_line:
    push    ebp
    mov     ebp, esp
    sub     esp, 8

    mov     dword [ebp-8], 0

_read_repeat:
    mov     eax, 3
    mov     ebx, [ebp+8]
    lea     ecx, [ebp-4]
    mov     edx, 1
    int     80h

    cmp     byte [ebp-4], 0Ah
    je      _end_read_line

    mov     ebx, read_buffer
    add     ebx, [ebp-8]
    movzx   eax, byte [ebp-4]
    mov     byte [ebx], al

    inc     dword [ebp-8]
    jmp     _read_repeat

_end_read_line:
    mov     eax, [ebp-8]
    mov     esp, ebp
    pop     ebp
    ret

; ebp + 8 - line length
; ebp - 4 - multiplier
; ebp - 8 - just 10
_parse_line:
    push    ebp
    mov     ebp, esp
    sub     esp, 8
    
    mov     dword [ebp-8], 10

    mov     ecx, [ebp+8]
    dec     ecx
    mov     dword [ebp-4], 1

_multiply:

    movzx   eax, byte [read_buffer + ecx]
    sub     eax, 30h
    mul     dword [ebp-4]
    
    cmp     byte [read_buffer], '+'
    jne      _sub
    
    add     dword [sum], eax
    xor     eax, eax

_sub:
    sub     dword [sum], eax 

    mov     eax, dword [ebp-4]
    mul     dword [ebp-8]
    mov     dword [ebp-4], eax

    loop    _multiply

    mov     esp, ebp
    pop     ebp
    ret
