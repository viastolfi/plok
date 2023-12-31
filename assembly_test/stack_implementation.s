.global _main
.align 2
_main:
    # stack 2 on the top of the stack
    mov x0, 2
    stp x0, xzr, [sp, -16]!

    # stack 3 on the top of the stack
    mov x0, 3
    stp x0, xzr, [sp, -16]!

    # get the value on the top of the stack and stock it in x1
    # x1 = 3
    # the stack pointer move 16 bits ahead
    # the head of the stack is now "2"
    ldr x1, [sp], 16

    # get the value on the top of the stack and stock it in x0
    # x0 = 2
    ldr x0, [sp], 16

    # now the stack pointer is at the same place it had when we started te program
    # if we try to get the value of the stack pointer we get 0
    # for example:
    # ldr x0, [sp], 16
    # x0 = 16

    # return 2
    mov x16, 1
    svc 128