# README 

## What is this repository for?

This repository features PRS courses.

## What is this directory for?

This directory features the first set of exercises. 

## Quick summary

These exercises consist of an introduction to input and output.

The following system calls are covered:

- [open](https://man7.org/linux/man-pages/man2/open.2.html)
- [dup](https://man7.org/linux/man-pages/man2/dup.2.html)
- [dup2](https://man7.org/linux/man-pages/man2/dup.2.html)
- [lseek](https://man7.org/linux/man-pages/man2/lseek.2.html)
- [read](https://man7.org/linux/man-pages/man2/read.2.html)
- [write](https://man7.org/linux/man-pages/man2/write.2.html)
- [close](https://man7.org/linux/man-pages/man2/close.2.html)

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

