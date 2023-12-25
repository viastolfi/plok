.global _main
.align 2

_main:
    sub sp, sp, 32
    mov x0, 16 
    mov x1, 12
    str x0, [sp]
    str x1, [sp]
    
    ldr x0, [sp]
    ldr x0, [sp, (-1 * 8)]

    mov x16, 1
    svc 128

