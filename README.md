# README 

## What is this repository for?

This repository features PRS courses.

## Quick summary

This repository contains a set of introductory exercises 
to system programming under `Linux` organized as follows:

| Directories          | Description                                                           |
| :------------------- | :-------------------------------------------------------------------- |
| [doc](./doc)         | `Doxygen` documentation of exercises (`td-1` to `td-6`).              |
| [doxygen](./doxygen) | `Doxygen` configuration files.                                        |
| [td-1](./td-1)       | Introductory exercises to inputs and outputs using system calls.      |
| [td-2](./td-2)       | Introductory exercises to processes creation using system calls.      |
| [td-3](./td-3)       | Introductory exercises to processes communication using system calls. |
| [td-4](./td-4)       | Introductory exercices to signals and stack manipulation using system calls. |
| [td-5](./td-5)       | Introductory exercises to threads using `pthread`.                    |
| [td-6](./td-6)       | Introductory exercises to `OpenMP`.                                   |

Additional information is available in the various directories.

## Prerequisites

### Common prerequisites

Install the following prerequisites:

* [GNU GCC](https://gcc.gnu.org/)
* [GNU Make](https://www.gnu.org/software/make/)
* [GNU GDB](https://www.sourceware.org/gdb/)

```
sudo apt-get install gcc make gdb
```

### Additional prerequisites

Install the following prerequisites in order to modify the `doxygen`
documentation:

* [Doxygen](https://doxygen.nl/)
* [Graphviz](https://graphviz.org/)

```
sudo apt-get install doxygen graphviz
```

## How do I get setup?

### Build the exercises

A `Makefile` file is available within each directory corresponding 
to a set of exercises. Navigate to the desired `td-*` directory and invoke 
`make` to build all the exercises of the directory:

```
cd <td_number> && make
```

A `bin` and an `obj` directory will be generated. The `bin` directory contains
all the executables while the `obj` directory contains object files.

To a source file corresponds an executable with the same name
(except for nested directories).

### Build the documentation of the exercises

Navigate to the [doxygen](./doxygen) directory and invoke `doxygen`:

```
cd doxygen && doxygen doxygen.conf
```

### Execute an exercise

Nagivate to the generated `bin` directory of a set of exercises 
and execute the desired exercise:

```
cd bin && ./<exercise>
```

### Consult the documentation of the exercises

Navigate to the `doc` directory located at the root of this project and 
open the `index.html` file.

