.global _main
.align 2
_main:
    # create stack
    sub sp, sp, 32

    # create variable
    mov x1, 42
    str x1, [sp, 0]
    mov x1, 2
    str x1, [sp, 1]

    # do addition
    ldr x0, [sp, 1]
    ldr x2, [sp, 0]
    add x0, x0, x2

    # free the stack
    add sp, sp, 32

    # return the value
    mov x16, 1
    svc 128

.LC0:
    .string "%i\n"