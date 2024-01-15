.global _main
.align 2

_main:
    # Allocate memory to the stack pointer
    # The number of byte that can be allocated as to be a multiple of 16
    sub sp, sp, #16
   
    # We store 15 in the x1 register then in the stack
    mov x1, #257
    str x1, [sp, #16]
    mov x2, #12
    str x2, [sp, #12]
    ldr x0, [sp, #16]

    add sp, sp, #16

    ret
