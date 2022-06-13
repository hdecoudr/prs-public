/*!
 * \ingroup td_2_group
 * \file childfirst.c
 * \brief Exercise 2.1
 *
 * Executes a child process first.
 *
 * This program uses the following system calls:
 * 
 *  - [pipe(int pipefd[2])](https://man7.org/linux/man-pages/man2/pipe.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [read(int fd, void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/read.2.html)
 *  - [write(int fd, const void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/write.2.html)
 *  - [close(int fd)](https://man7.org/linux/man-pages/man2/close.2.html) 
 *
 * \author H. Decoudras
 * \version 1
 */


#include <sys/types.h>
#include <sys/wait.h>
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
 *  - [pipe(int pipefd[2])](https://man7.org/linux/man-pages/man2/pipe.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [read(int fd, void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/read.2.html)
 *  - [write(int fd, const void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/write.2.html)
 *  - [close(int fd)](https://man7.org/linux/man-pages/man2/close.2.html
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *            in case of error
 */
int main(void)
{
    /*
        fd[0]: read
        fd[1]: write
     */

    int fd[2];
    
    int result = pipe(fd);
    exit_on_error(result < 0);

    pid_t child_pid = fork();
    exit_on_error(child_pid < 0);

    ssize_t rw_result;

    if (child_pid)
    {
        /* Parent process */

        /* Close the write side of the pipe */

        result = close(fd[1]);
        exit_on_error(errno);

        /* Block the parent execution */

        int value;
        while ((rw_result = read(fd[0], &value, sizeof(int))) == 0)
        {
            sleep(1);
        }

        exit_on_error(rw_result < 0);

        fprintf(
            stdout, 
            "My name is [%d] and I am the father of [%d]\n", 
            getpid(), 
            child_pid
        );

        /* Close the read side of the pipe */

        result = close(fd[0]);
        exit_on_error(result < 0);

        /* Wait for the child process to finish */

        int status;
        pid_t wait_result = wait(&status);
        exit_on_error(wait_result < 0);
    }
    else
    {   
        /* Child process */

        /* Close the read side of the pipe */

        result = close(fd[0]);
        exit_on_error(result < 0);

        fprintf(
            stdout, 
            "My name is [%d] and I am the child of [%d]\n", 
            getpid(), 
            getppid()
        ); 

        /* Unlock the parent process */

        int value = 1;
        rw_result = write(fd[1], &value, sizeof(int));
        exit_on_error(rw_result < 0);

        /* Close the write side of the pipe */

        result = close(fd[1]);
        exit_on_error(result < 0);
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

