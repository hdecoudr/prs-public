# README 

## What is this repository for?

This repository features PRS courses.

## What is this directory for?

This directory features a shell interpreter that execute 
shell commands. 

> :warning: This program is buggy and needs further profiling

## Prerequisites

Install the following prerequisites:

* [GNU GCC](https://gcc.gnu.org/)
* [GNU Make](https://www.gnu.org/software/make/)
* [GNU GDB](https://www.sourceware.org/gdb/)
* [Flex](https://github.com/westes/flex)
* [GNU Bison](https://www.gnu.org/software/bison/)
* [GNU Readline](https://tiswww.case.edu/php/chet/readline/rltop.html)

```
sudo apt-get install gcc make gdb flex bison libreadline-dev
```

## How do I get setup?

### Build the exercises

Invoke `make` to build the exercise:

```
make
```

> :pushpin: By default, the exercise is compiled in debug mode. 
  Modify the `Makefile` file by removing the `-g` flag to compile 
  them in release mode.

### Run the exercises

The `make` command will create the `shelltree` executable. Execute it:

```
./shelltree
```

