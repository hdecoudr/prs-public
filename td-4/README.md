# README 

## What is this repository for?

This repository features PRS courses.

## What is this directory for?

This directory features the fourth set of exercises. 

These exercises consist of an introduction to signals and stack manipulation.

The following system calls and functions are covered:

- [sigprocmask](https://man7.org/linux/man-pages/man2/sigprocmask.2.html)
- [sigaction](https://man7.org/linux/man-pages/man2/sigaction.2.html)
- [sigsuspend](https://man7.org/linux/man-pages/man2/sigsuspend.2.html)
- [fork](https://man7.org/linux/man-pages/man2/fork.2.html)
- [waitpid](https://man7.org/linux/man-pages/man2/wait.2.html)
- [kill](https://man7.org/linux/man-pages/man2/kill.2.html)
- [pause](https://man7.org/linux/man-pages/man2/pause.2.html)
- [execvp](https://man7.org/linux/man-pages/man3/exec.3.html)
- [sigemptyset](https://man7.org/linux/man-pages/man3/sigemptyset.3p.html)
- [sigaddset](https://man7.org/linux/man-pages/man3/sigaddset.3p.html)
- [setjmp](https://man7.org/linux/man-pages/man3/setjmp.3.html)
- [longjmp](https://man7.org/linux/man-pages/man3/longjmp.3p.html)

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

