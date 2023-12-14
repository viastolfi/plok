.global _main
.align 2

_main:
    sub sp, sp, 32 // Allocate space for the whole block
    mov x1, 42 // Add value 4 on the x1 register
    str x1, [sp, 0] // write the value of x1 to slot 11
    ldr x0, [sp, 0] // read from slot 11 into x0
    add sp, sp, 32 // Free the space at the end of the block

    mov x16, 1
    svc 128