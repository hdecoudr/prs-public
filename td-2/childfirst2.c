/*!
 * \ingroup td_2_group
 * \file childfirst2.c
 * \brief Exercise 2.1
 *
 * Executes a child process first.
 *
 * This program uses the following system calls:
 * 
 *  - [mmap(void\* addr, size_t length, int prot, int flags, int fd, off_t offset)](https://man7.org/linux/man-pages/man2/mmap.2.html)
 *  - [munmap(void\* addr, size_t length)](https://man7.org/linux/man-pages/man2/mmap.2.html)
 *  - [pipe(int pipefd[2])](https://man7.org/linux/man-pages/man2/pipe.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *
 * \author H. Decoudras
 * \version 1
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


/*!
 * \brief The exit_on_error() function exits the program
 *        if the \p assertion parameter is evaluated
 *        to `TRUE`.
 *
 * If the assertion is evaluated to `TRUE` and
 * [errno](https://man7.org/linux/man-pages/man3/errno.3.html)
 * is set, then the error number and its associated message
 * are displayed. Otherwise, a generic message is displayed.
 *
 * \param assertion Assertion to be evaluated.
 */
static void exit_on_error(int assertion);


/*!
 * \brief Main entry point of the program.
 *
 * Executes a child process first.
 *
 * This program uses the following system calls:
 * 
 *  - [mmap(void\* addr, size_t length, int prot, int flags, int fd, off_t offset)](https://man7.org/linux/man-pages/man2/mmap.2.html)
 *  - [munmap(void\* addr, size_t length)](https://man7.org/linux/man-pages/man2/mmap.2.html)
 *  - [pipe(int pipefd[2])](https://man7.org/linux/man-pages/man2/pipe.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *            in case of error
 */
int main(void)
{
    /* Create a shared object */

    int* shared = mmap(
        NULL, 
        sizeof(int), 
        PROT_READ | PROT_WRITE, 
        MAP_SHARED | MAP_ANON,
        -1,
        0
    );
    exit_on_error(shared == MAP_FAILED);
    
    *shared = 0;

    pid_t child_pid = fork();
    exit_on_error(child_pid < 0);

    if (child_pid)
    { 
        /* Parent process */

        /* Block the parent execution */

        while (*shared != 1)
        {
            fprintf(stdout, "Waiting for child execution!\n");
            sleep(1);
        }

        int status;
        fprintf(
            stdout, 
            "My name is [%d] and I am the father of [%d]\n", 
            getpid(), 
            child_pid
        );

        /* Wait for the child process to finish */

        pid_t wait_result = wait(&status);
        exit_on_error(wait_result < 0);

        int result = munmap(shared, sizeof(int));
        exit_on_error(result < 0);
    }
    else
    {    
        /* Child process */

        fprintf(
            stdout, 
            "My name is [%d] and I am the child of [%d]\n", 
            getpid(), 
            getppid()
        ); 

        /* Unlock the parent process */

        *shared = 1;
    }

    return EXIT_SUCCESS;
}


void exit_on_error(int assertion)
{
    if (assertion)
    {
        if (errno)
        {
            fprintf(stderr, "[%d]: %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }

        fprintf(stderr, "An error occured!\n");
        exit(EXIT_FAILURE);
    }
}

