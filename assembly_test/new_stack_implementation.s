.global _main
.align 2

_main:
    # Allocate memory to the stack pointer
    # The number of byte that can be allocated as to be a multiple of 16
    sub sp, sp, #16
   
    # We store 15 in the x1 register then in the stack
    mov x1, 256
    str x1, [sp, #16]
    mov x1, 42
    str x1, [sp, #15]
    ldr x0, [sp, #16]

    add sp, sp, #16

    ret
