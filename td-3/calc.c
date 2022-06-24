/*!
 * \ingroup td_3_group
 * \file calc.c
 * \brief Exercise 3.6
 *
 * Sequentially distributes a calculation across several processes.
 *
 * This program uses the following system calls:
 *
 *  - [pipe(int pipefd[2])](https://man7.org/linux/man-pages/man2/pipe.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [open(const char\* pathname, int flags, mode_t mode)](https://man7.org/linux/man-pages/man2/open.2.html)
 *  - [dup2(int oldfd, int newfd)](https://man7.org/linux/man-pages/man2/dup.2.html)
 *  - [write(int fd, const void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/write.2.html)
 *  - [close(int fd)](https://man7.org/linux/man-pages/man2/close.2.html)
 *
 * \author H. Decoudras
 * \version 1
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*!
 * \brief Number of child processes.
 */
#define N 4


/*!
 * \brief Functions to be launched for each process.
 */
typedef void (*func_ptr)(double*);

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
 * \brief The read_next_double() function reads a double
 *        from the standard input.
 *
 * \param value Value to read.
 *
 * \return This function can return the following values:
 *          - **0** if a double has been read
 *          - **-1** if nothing has been read
 */
static int read_next_double(double* value);


/*!
 * \brief First function.
 *
 * \param value Value.
 */
static void g1(double* value);

/*!
 * \brief Second function.
 *
 * \param value Value.
 */
static void g2(double* value);

/*!
 * \brief Third function.
 *
 * \param value Value.
 */
static void g3(double* value);

/*!
 * \brief Fourth function.
 *
 * \param value Value.
 */
static void g4(double* value);


/*!
 * \brief Functions to be launched for each process.
 */
static func_ptr func[] = {g1, g2, g3, g4};


/*!
 * \brief Main entry point of the program.
 *
 * Sequentially distributes a calculation across several processes.
 *
 * This program uses the following system calls:
 * 
 *  - [pipe(int pipefd[2])](https://man7.org/linux/man-pages/man2/pipe.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [open(const char\* pathname, int flags, mode_t mode)](https://man7.org/linux/man-pages/man2/open.2.html)
 *  - [dup2(int oldfd, int newfd)](https://man7.org/linux/man-pages/man2/dup.2.html)
 *  - [write(int fd, const void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/write.2.html)
 *  - [close(int fd)](https://man7.org/linux/man-pages/man2/close.2.html)
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *            in case of error
 */
int main(void)
{
    int     fd[N][2];
    int     result;
    ssize_t rw_result;
    pid_t   wait_result;

    for (int i = 0; i < N; ++i)
    {
        result = pipe(fd[i]);
        exit_on_error(result < 0);
    }

    double value;

    /* Chain processes */

    for (int i = 0; i < N; ++i)
    {
        pid_t child_pid = fork();
        exit_on_error(child_pid < 0);
        
        if (!child_pid) 
        {        
            /* Child process */

            /* 
                Redirect the read side of the pipe to the
                standard input

                The standard input is silently closed and
                its file descriptor points to the read side
                of the pipe
                
                Do it except for the first process
                (already redirected when calling the program)
             */
                 
            if (i)
            {
                result = dup2(fd[i][0], STDIN_FILENO);
                exit_on_error(result < 0);
            }

            /* 
                Redirect the write side of the pipe to the
                standard output

                The standard output is silently closed and
                its file descriptor points to the write side
                of the pipe
                
                Do it except for the first process
                (already redirected when calling the program)
             */

            if (i != (N - 1))
            {
                result = dup2(fd[i + 1][1], STDOUT_FILENO);
                exit_on_error(result < 0);
            }

            /*
                Close the read side and the write side
                of the pipe
             */

            for (int j = 0; j < N; ++j)
            {
                result = close(fd[j][0]);
                exit_on_error(result < 0);

                result = close(fd[j][1]);
                exit_on_error(result < 0);
            }

            while ((rw_result = read_next_double(&value)) != -1)
            {       
                func[i](&value);
                rw_result = write(STDOUT_FILENO, &value, sizeof(double));
                exit_on_error(rw_result < 0);
            } 

            exit(EXIT_SUCCESS);
        }
    }

    /* Fail if we wait all children */

    wait_result = wait(NULL);
    exit_on_error(wait_result < 0);    
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

int read_next_double(double* value)
{
    if (read(STDIN_FILENO, value, sizeof(double)) <= 0)
    {
        return -1;
    }

    return 0;
}

void g1(double* value)
{
    *value += .1;    
}

void g2(double* value)
{
    *value += .01;
}

void g3(double* value)
{
    *value += .001;
}

void g4(double* value)
{
    *value += .0001;
}

