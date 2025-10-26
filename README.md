# fuckUNIX

<hr>

A OS based on FrOS by FRosner

## Description

This is a simple x86 32 bit OS based on FrOS by FRosner. For now it has the following components:

1. Kernel
2. IO
3. VGA Text output
4. A basic command line
5. Keyboard support
6. Dynamic memory
7. A broken ATA/IDE driver
8. PIC timing and interupts

## Setup

## Linux and Mac:

### Install Assembler and Emulator

```bash
brew install qemu nasm
```

### Install Kernel Compiler

https://github.com/lordmilko/i686-elf-tools#mac-os-x

```
brew install i386-elf-binutils
brew install i386-elf-gcc
brew install i386-elf-gdb
```

```
export PATH="/usr/local/Cellar/x86_64-elf-binutils/<version>/bin/:/usr/local/Cellar/x86_64-elf-gcc/<version>/bin/:/usr/local/Cellar/i386-elf-gdb/<version>/bin:$PATH"
```

# Windows

## Installing NASM
To install NASM u will need to go to [the NASM website](https://www.nasm.us/) and get the setup executable,
then you need to run it.

## Installing the Cross Compiler
To install the cross compiler i recomend [this repo](https://github.com/lordmilko/i686-elf-tools/releases/tag/15.2.0), when you are on the page you will see a few zip archives get the one titled ```x86_64-elf-tools-windows.zip```.

Extract the zip somewhere where u can acsess it, remember the path.

## Adding it to path
To add the compiler to path press the windows key and search for: Edit the system envirment variables, once open click on the Enviroment Variables button in the corner. Then find and select the PATH variable then click on the Edit button,
after it opened the menu click on the Edit button.

Now copy paste the path from before like so ```C:/x86_64-gcc/bin``` once its in path test it out by opening a new terminal and typing in ```x86_64-elf-gcc```, the output should be something like: ```x86_64-elf-gcc: fatal error: no input files
compilation terminated.```

## Usage

### Boot

```
make run
```

### Debug

```
make debug
```

In GDB shell:

- Set breakpoint at function (e.g. `start_kernel`): `b start_kernel`
- Start execution: `c`
- Jump to next instruction: `n`
- Print variable: `p <variable_name>`

## Additional Resources

- [Writing a Simple Operating System — from Scratch](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf)
- [OS Dev Wiki](https://wiki.osdev.org/Meaty_Skeleton)
- [JamesM's Kernel Development Tutorials](https://web.archive.org/web/20160412174753/http://www.jamesmolloy.co.uk/tutorial_html/index.html)
- [BrokenThorn Entertainment's Operating System Development Series](http://www.brokenthorn.com/Resources/OSDevIndex.html)
- [The Little Book About OS Development](https://littleosbook.github.io/)
- [C Memory Allocation Functions](http://www.sunshine2k.de/articles/coding/cmemalloc/cmemory.html)
- [FrOS](https://github.com/FRosner/FrOS)
