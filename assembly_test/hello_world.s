.global _main
.align 2 

_main:
    b _hello_world
    b _terminate
_hello_world:
    mov X0, 1
    adrp X1, hello_txt@PAGE
    add X1, X1, hello_txt@PAGEOFF
    mov X2, 14
    mov X16, 4
    svc 128

_terminate:
    mov x16, 1
    mov x0, XZR
    svc 128
.data
hello_txt: .ascii "Hello World !\n"
