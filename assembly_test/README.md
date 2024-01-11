# Some explanaition on how assembly work in arm64

Firstly, go check the files I've create they contain lot of test I've done in assembly and can explain some simple concept. \

## Stack pointer

The most important thing to understand is the stack and the stack pointer

There are some of the things to know.

You can allocate memory space to your stack by using the stacj pointer `sp` before storing value in it to optimize your program

```asm
sub sp, sp, #bytes
```

The number of bytes you can allocate as to be a multiple of 16 (16, 32, 48 ...)

Then you can store value in the stack. You have to know that each location of the stack can contains a value of exactly one byte. \
A byte can be a value between 0 and 255. That means that if you want to stack 256 in the stack you have to use two place in the stack \

Weach means :

```asm
sub sp, sp, #16
mov x1, 256
str x1, [sp, #16]
; now if you want to add a new value you have to place it at #14 location
; otherwise :
mov x1, 123 ;it works with every value
str x1, [sp, #15]
; now if you get the value at the #15 value you indeed get '123'
; but : 
str x1, [sp, #16] ; now return 0 beacause the value get ereased
```

So depending on the value you want to store in your stack you have to use more or less location from the stack
