/*!
 * \ingroup td_3_group
 * \file calc2.c
 * \brief Exercise 3.6
 *
 * Distributes by block a calculation through several processes.
 *
 * This program uses the following system calls:
 *
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [open(const char\* pathname, int flags, mode_t mode)](https://man7.org/linux/man-pages/man2/open.2.html)
 *  - [lseek(int fd, off_t offset, int whence)](https://man7.org/linux/man-pages/man2/lseek.2.html)
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
#include <float.h>


/*!
 * \brief Number of child processes.
 */
#define N 4

/*!
 * \brief Functions to be launched for each process.
 */
typedef void (*func_ptr)(double*);


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
 * \brief Sample function
 *
 * \param value Value.
 */
static void f(double* value);


/*!
 * \brief Main entry point of the program.
 *
 * Distributes by block a calculation through several processes.
 *
 * This program uses the following system calls:
 *
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [open(const char\* pathname, int flags, mode_t mode)](https://man7.org/linux/man-pages/man2/open.2.html)
 *  - [lseek(int fd, off_t offset, int whence)](https://man7.org/linux/man-pages/man2/lseek.2.html)
 *  - [write(int fd, const void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/write.2.html)
 *  - [close(int fd)](https://man7.org/linux/man-pages/man2/close.2.html)
 *
 * \param argc Number of arguments of the program.
 * \param argv Arguments of the program.
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *            in case of error
 */
int main(int argc, char** argv)
{
    exit_on_argv_error(argc, argv);

    /* Open the input file in read only mode */

    int fd_in = open(argv[1], O_RDONLY);
    exit_on_error(fd_in < 0);

    /* 
        Open the output file in write only mode

        Create the file if it does not exist and trunc
        its content
    */ 

    int fd_out = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0666);
    exit_on_error(fd_out < 0);

    /* Get the size of the input file */

    off_t fd_in_size = lseek(fd_in, 0, SEEK_END);
    exit_on_error(fd_in_size < 0);

    /* Go back to the begining of the input file */

    off_t seek_result = lseek(fd_in, 0, SEEK_SET);
    exit_on_error(seek_result < 0);

    double  value;
    off_t  element_count = fd_in_size / sizeof(double);
    off_t  block_size = element_count / N;
    ssize_t rw_result;

    /* Distribute the calculation */

    for (int i = 0; i < N; ++i)
    {
        pid_t child_pid = fork();
        exit_on_error(child_pid < 0);
        
        if (!child_pid) 
        {
            /* Child process */

            /* 
                File descriptors are shared after a fork
                
                A write or read from a process will affect 
                the next write or read offset for others

                Generate new ones
            */ 

            int fd_in = open(argv[1], O_RDONLY);
            exit_on_error(fd_in < 0);
            
            int fd_out = open(argv[2], O_CREAT | O_WRONLY, 0666);
            exit_on_error(fd_out < 0);

            /* Go to begining of a block */

            seek_result = lseek(
                fd_in, 
                i * block_size * sizeof(double), 
                SEEK_SET
            );
            exit_on_error(seek_result < 0);

            seek_result = lseek(
                fd_out, 
                i * block_size * sizeof(double), 
                SEEK_SET
            );
            exit_on_error(seek_result < 0);
            
            /* Write to the block */

            for (off_t j = 0; j < block_size; ++j)
            {
                rw_result = read(fd_in, &value, sizeof(double));
                exit_on_error(rw_result < 0);

                f(&value);

                rw_result = write(fd_out, &value, sizeof(double));
                exit_on_error(rw_result < 0);
            }

            exit(EXIT_SUCCESS);
        }
    }

    /* Parent process */

    /* If there is any remaining data to write */

    off_t remainder = element_count % N; 

    if (remainder)
    {
        seek_result = lseek(
            fd_in, 
            N * block_size * sizeof(double), 
            SEEK_SET
        );
        exit_on_error(seek_result < 0);

        seek_result = lseek(
            fd_out, 
            N * block_size * sizeof(double), 
            SEEK_SET
        );
        exit_on_error(seek_result < 0);

        for (int i = 0; i < element_count % N; ++i)
        {
            rw_result = read(fd_in, &value, sizeof(double));
            exit_on_error(rw_result < 0);

            f(&value);

            rw_result = write(fd_out, &value, sizeof(double));
            exit_on_error(rw_result < 0);
        }
    }
    
    /* Wait for the child processes to finish */

    pid_t wait_result;
    for (int i = 0; i < N; ++i)
    {
        int status;
        wait_result = wait(&status);
        exit_on_error(wait_result < 0);
    }
    
    return EXIT_SUCCESS;
}


void use(const char* program)
{
    fprintf(
        stderr, 
        "Use:\n  %s <filename_in> <filename_out>\n", 
        program
    );
    exit(EXIT_FAILURE);
}

void exit_on_argv_error(int argc, char** argv)
{
    if (argc != 3)
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
        exit(EXIT_FAILURE);
    }
}

void f(double* value)
{
    *value += .1;    
}

