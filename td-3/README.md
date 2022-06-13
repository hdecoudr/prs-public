# README 

## What is this repository for?

This repository features PRS courses.

## What is this directory for?

This directory features the third set of exercises. 

These exercises consist of an introduction to communication between processes.

The following system calls and functions are covered:

- [pipe](https://man7.org/linux/man-pages/man2/pipe.2.html)
- [pipe2](https://man7.org/linux/man-pages/man2/pipe.2.html)
- [fork](https://man7.org/linux/man-pages/man2/fork.2.html)
- [wait](https://man7.org/linux/man-pages/man2/wait.2.html)
- [open](https://man7.org/linux/man-pages/man2/open.2.html)
- [dup2](https://man7.org/linux/man-pages/man2/dup.2.html)
- [lseek](https://man7.org/linux/man-pages/man2/lseek.2.html)
- [read](https://man7.org/linux/man-pages/man2/read.2.html)
- [write](https://man7.org/linux/man-pages/man2/write.2.html)
- [close](https://man7.org/linux/man-pages/man2/close.2.html)
- [execl](https://man7.org/linux/man-pages/man3/exec.3.html)
- [execlp](https://man7.org/linux/man-pages/man3/exec.3.html)
- [execvp](https://man7.org/linux/man-pages/man3/exec.3.html)

## Prerequisites

Install the following prerequisites:

* [GNU GCC](https://gcc.gnu.org/)
* [GNU Make](https://www.gnu.org/software/make/)
* [GNU GDB](https://www.sourceware.org/gdb/)

```sh
sudo apt-get install gcc make gdb
```

## How do I get setup?

### Build the exercises

Run the following command to build the exercises:

```sh
make
```

> :pushpin: By default, the exercises are compiled in debug mode. 
  Modify the `Makefile` file by removing the `-g` flag to compile 
  them in release mode.

### Run the exercises

Navigate to the `bin` directory and execute an exercise:

```sh
./<name_of_the_executable>
```

