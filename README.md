# plok

Plok is a minimalist programming langage that i'm trying to create on my free time \
The main goal of this project is to learn a bit of assembly and c++ while doing something fun \
At the moment it's only working on arm64 (and maybe only in mac)

# Installation

At the moment you can only use it by cloning the repo

`https://github.com/viastolfi/plok.git`

Then you can build the project by running those commands 

```
cd plok
mkdir build
cmake -S . -B build/
cmake --build build/
./build/plok your_plok_file
./build/out
```

# Basics

At the moment in plok you can 

```
return something,
print something,
let a variable and use it as a return code for example
```

My goal is not really to make a working and usable programming langage but to train and challenge myself for a bit \
By the way, I know that there is a lot of mistake done in the code or in the way my compiler work but it's okay I will improve it later (no)

# Acknowledgement

This project is mainly inspired by [this youtube series](https://www.youtube.com/playlist?list=PLUDlas_Zy_qC7c5tCgTMYq2idyyT241qs) \
[Documentation for syscall instruction number](https://opensource.apple.com/source/xnu/xnu-1504.3.12/bsd/kern/syscalls.master) \
[Documentation to learn asm in arm64](https://modexp.wordpress.com/2018/10/30/arm64-assembly/)
