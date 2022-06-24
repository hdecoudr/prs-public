# README 

## What is this repository for?

This repository features PRS courses.

## What is this directory for?

This directory features the second set of exercises. 

## Quick summary

These exercises consist of an introduction to process creation.

The following system calls and functions are covered:

- [mmap](https://man7.org/linux/man-pages/man2/mmap.2.html)
- [munmap](https://man7.org/linux/man-pages/man2/mmap.2.html)
- [pipe](https://man7.org/linux/man-pages/man2/pipe.2.html)
- [fork](https://man7.org/linux/man-pages/man2/fork.2.html)
- [wait](https://man7.org/linux/man-pages/man2/wait.2.html)
- [waitpid](https://man7.org/linux/man-pages/man2/wait.2.html)
- [kill](https://man7.org/linux/man-pages/man2/kill.2.html)
- [raise](https://man7.org/linux/man-pages/man3/raise.3.html)
- [read](https://man7.org/linux/man-pages/man2/read.2.html)
- [write](https://man7.org/linux/man-pages/man2/write.2.html)
- [close](https://man7.org/linux/man-pages/man2/close.2.html)
- [execvp](https://man7.org/linux/man-pages/man3/exec.3.html)
- [system](https://man7.org/linux/man-pages/man3/system.3.html)

The [shelltree](./shelltree) directory also provides an introduction to 
[Bison](https://www.gnu.org/software/bison/) and 
[Flex](https://github.com/westes/flex).

## Prerequisites

Install the following prerequisites:

* [GNU GCC](https://gcc.gnu.org/)
* [GNU Make](https://www.gnu.org/software/make/)
* [GNU GDB](https://www.sourceware.org/gdb/)

```
sudo apt-get install gcc make gdb
```

## How do I get setup?

### Build the exercises

Run the following command to build the exercises:

```
make
```

> :pushpin: By default, the exercises are compiled in debug mode. 
  Modify the `Makefile` file by removing the `-g` flag to compile 
  them in release mode.

### Run the exercises

Navigate to the `bin` directory and execute an exercise:

```
./<name_of_the_executable>
```

