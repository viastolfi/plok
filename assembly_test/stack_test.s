.global _main
.align 2

_main:
    mov x0, 16
    mov x1, 12
    stp x0, xzr, [sp, -16]!
    stp x1, xzr, [sp, -16]!

    ldr x0, [sp], 16
    ldr x1, [sp], 16

    mov x16, 1
    svc 128

