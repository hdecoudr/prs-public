# README 

## What is this repository for?

This repository features PRS courses.

## What is this directory for?

This directory features the sixth set of exercises. 

These exercises consist of an introduction to [OpenMP](https://www.openmp.org/).

The following directives and functions are covered:

- [parallel](https://www.openmp.org/spec-html/5.0/openmpse14.html#x54-800002.6)
- [barrier](https://www.openmp.org/spec-html/5.0/openmpsu90.html#x121-4550002.17.2)
- [critical](https://www.openmp.org/spec-html/5.0/openmpsu89.html#x120-4470002.17.1)
- [atomic](https://www.openmp.org/spec-html/5.0/openmpsu95.html#x126-4840002.17.7)
- [omp_get_thread_num]https://www.openmp.org/spec-html/5.0/openmpsu113.html#x150-6570003.2.4)
- [omp_get_thread_num]https://www.openmp.org/spec-html/5.0/openmpsu113.html#x150-6570003.2.4)

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

