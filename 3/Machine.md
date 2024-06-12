## Machine Code Explanation for c4.c

### Function `power(int a, int n)`

```assembly
    ENT 1                   ; Enter subroutine with 1 parameter
    LLA -1                  ; Load local address of r
    PSH                     ; Push r onto the stack
    LLA 3                   ; Load local address of a
    LI                      ; Load value at address a
    SI                      ; Store value at address r
    LLA 2                   ; Load local address of n
    LI                      ; Load value at address n
    PSH                     ; Push value onto the stack
    IMM 0                   ; Load immediate value 0
    EQ                      ; Compare if n equals 0
    BZ loc_1                ; Branch if zero to loc_1
    IMM 1                   ; Load immediate value 1
    LEV                     ; Leave subroutine

loc_1:
    LLA 2                   ; Load local address of n
    LI                      ; Load value at address n
    PSH                     ; Push value onto the stack
    IMM 1                   ; Load immediate value 1
    SUB                     ; Subtract 1 from n
    PSH                     ; Push updated value of n onto the stack
    IMM 0                   ; Load immediate value 0
    GT                      ; Compare if (n-1) is greater than 0
    BZ loc_2                ; Branch if zero to loc_2
    LLA 3                   ; Load local address of a
    PSH                     ; Push a onto the stack
    LLA 3                   ; Load local address of a
    LI                      ; Load value at address a
    PSH                     ; Push value onto the stack
    LLA -1                  ; Load local address of r
    LI                      ; Load value at address r
    MUL                     ; Multiply a by r
    SI                      ; Store result in a
    LLA 2                   ; Load local address of n
    PSH                     ; Push n onto the stack
    LLA 2                   ; Load local address of n
    LI                      ; Load value at address n
    PSH                     ; Push value onto the stack
    IMM 1                   ; Load immediate value 1
    SUB                     ; Subtract 1 from n
    SI                      ; Store updated value of n
    JMP loc_1               ; Jump to loc_1

loc_2:
    LLA 3                   ; Load local address of a
    LI                      ; Load value at address a
    LEV                     ; Leave subroutine
    LEV                     ; Leave subroutine