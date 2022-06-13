/*!
 * \ingroup td_3_group
 * \file truepipe.c
 * \brief Exercise 3.2
 *
 * Executes a two commands by using a pipe.
 *
 * This program uses the following system calls and functions: 
 *
 *  - [pipe(int pipefd[2])](https://man7.org/linux/man-pages/man2/pipe.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html) 
 *  - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [dup2(int oldfd, int newfd)](https://man7.org/linux/man-pages/man2/dup.2.html)
 *  - [close(int fd)](https://man7.org/linux/man-pages/man2/close.2.html)
 *  - [execlp(const char\* file, const char\* arg, ..., NULL)](https://man7.org/linux/man-pages/man3/exec.3.html)
 *  - [execvp(const char\* file, char\* const argv[])](https://man7.org/linux/man-pages/man3/exec.3.html)
 *
 * \author H. Decoudras
 * \version 1
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*!
 * \brief The use() function displays how to use the
 *        program.
 *
 *        This function always exits the program.
 *
 * \param program Name of the program.
 */
static void use(const char* program);

/*!
 * \brief The exit_on_argv_error() function exits the
 *        program if the provided arguments are not
 *        valid.
 *
 *        This function calls the use() one.
 *
 * \param argc Number of arguments of the program.
 * \param argv Arguments of the program.
 *
 * \see use()
 */
static void exit_on_argv_error(int argc, char** argv);

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
 * Simulates a pipe between two commands by using a file.
 *
 * This program uses the following system calls and functions:
 * 
 *  - [pipe(int pipefd[2])](https://man7.org/linux/man-pages/man2/pipe.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html) 
 *  - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [dup2(int oldfd, int newfd)](https://man7.org/linux/man-pages/man2/dup.2.html)
 *  - [close(int fd)](https://man7.org/linux/man-pages/man2/close.2.html)
 *  - [execlp(const char\* file, const char\* arg, ..., NULL)](https://man7.org/linux/man-pages/man3/exec.3.html)
 *  - [execvp(const char\* file, char\* const argv[])](https://man7.org/linux/man-pages/man3/exec.3.html)
 *
 * \param argc Number of arguments of the program.
 * \param argv Arguments of the program.
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *            in case of error */
int main(int argc, char** argv)
{
    exit_on_argv_error(argc, argv);

    /*
        fd[0]: read
        fd[1]: write
     */

    int fd[2]; 
    int result = pipe(fd);
    exit_on_error(result < 0); 

    pid_t child_pid = fork();
    exit_on_error(child_pid < 0);
    
    if (!child_pid)
    {
        /* Child process */

        /* Close the write side */
        
        result = close(fd[1]);
        exit_on_error(result < 0);

        /* 
            Redirect the read side of the pipe to the
            standard input

            The standard input is silently closed and
            its file descriptor points to the read
            side of the pipe
        */
        
        result = dup2(fd[0], STDIN_FILENO);  
        exit_on_error(result < 0);

        /* 
            Close the read side of the pipe

            We will use STDIN_FILENO to refers to it
        */

        result = close(fd[0]);
        exit_on_error(result < 0);
        
        /* Execute the second command */

        result = execvp(argv[2], &argv[2]);
        exit_on_error(result < 0);
    }
    else
    {
        /* Parent process */
        
        /* Close the read side */
        
        result = close(fd[0]);    
        exit_on_error(result < 0);

        /* 
            Redirect the write side of the pipe to the
            standard output

            The standard output is silently closed and
            its file descriptor points to the standard
            output
        */
        
        result = dup2(fd[1], STDOUT_FILENO); 
        exit_on_error(result < 0);

        /* 
            Close the write side of the pipe

            We will use STDOUT_FILENO to refers to it
        */
        
        result = close(fd[1]);               
        exit_on_error(result < 0);

        /* Execute the first command */

        result = execlp(argv[1], argv[1], NULL);
        exit_on_error(result < 0);
    }

    return EXIT_SUCCESS;
}


void use(const char* program)
{
    fprintf(stderr, "Use:\n  %s <cmd_1> <cmd_2> [<arguments>]\n", program);
    exit(EXIT_FAILURE);
}

void exit_on_argv_error(int argc, char** argv)
{
    if (argc < 4)
    {
        use(argv[0]);
    }
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
    }
}

