/*!
 * \ingroup td_2_group
 * \file parentchild.c
 * \brief Exercise 2.1
 *
 * Creates a child process.
 *
 * This program uses the following system calls:
 *
 * - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 * - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *
 * \author H. Decoudras
 * \version 1
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
 * Creates a child process.
 *
 * This program uses the following system calls:
 *
 * - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 * - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *            in case of error
 */
int main(void)
{
    pid_t child_pid = fork();
    exit_on_error(child_pid < 0);

    if (child_pid)
    {   
        /* Parent process */

        fprintf(
            stdout, 
            "My name is [%d] and I am the father of [%d]\n", 
            getpid(), 
            child_pid
        );

        /* Wait for the child process to finish */

        int status;
        pid_t wait_result = wait(&status);
        exit_on_error(wait_result < 0);
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

