# README 

## What is this repository for?

This repository features PRS courses.

## What is this directory for?

This directory features the fifth set of exercises. 

## Quick summary

These exercises consist of an introduction to threads.

The following system calls are covered:
    
- [pthread_create](https://man7.org/linux/man-pages/man3/pthread_create.3.html)
- [pthread_join](https://man7.org/linux/man-pages/man3/pthread_join.3.html)
- [pthread_mutex_lock](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [pthread_mutex_unlock](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)

## Prerequisites

Install the following dependencies:

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

