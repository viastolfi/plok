.global _main
.align 2

_main:
    mov x0, 3
    mov x1, 1

    sub x0, x0, x1
    mov x16, 1
    svc 128
