/*!
 * \ingroup td_3_group
 * \file pipesize.c
 * \brief Exercise 3.3
 *
 * Computes the size of a pipe.
 * 
 * This program uses the following system calls: 
 * 
 *  - [pipe2(int pipefd[2], int flags)](https://man7.org/linux/man-pages/man2/pipe.2.html)
 *  - [write(int fd, const void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/write.2.html)
 *  - [close(int fd)](https://man7.org/linux/man-pages/man2/close.2.html)
 *
 * \author H. Decoudras
 * \version 1
 */

#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


/*!
 * \brief Callback function.
 */
typedef void (*error_callback)(ssize_t);


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
 * \param error_callback Function to call.
 * \param count Callback parameter.
 */
static void exit_on_error(int assertion, error_callback callback,
                          ssize_t count);


/*!
 * \brief The print_result() function is used
 *        to display the size of a pipe.
 *
 * \param value Size of the pipe.
 *
 * \see error_callback()
 */
static void print_result(ssize_t value);


/*!
 * \brief Main entry point of the program.
 *
 * Computes the size of a pipe.
 * 
 * This program uses the following system calls: 
 * 
 *  - [pipe2(int pipefd[2], int flags)](https://man7.org/linux/man-pages/man2/pipe.2.html)
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
    
    /*
        fd[0]: read
        fd[1]: write
     */

    int fd[2];
    
    /* 
        Make the write system call to fail 
        with errno set to EAGAIN if the pipe
        is full
    */
    
    int result = pipe2(fd, O_NONBLOCK);
    exit_on_error(result < 0, NULL, 0);

    long int    bytes = atol(argv[1]);
    ssize_t     count = 0;
    ssize_t     rw_result;

    for (long int i = 1; i <= bytes; ++i)
    {
        rw_result = write(fd[1], "a", sizeof(char));
        exit_on_error(rw_result < 0, &print_result, count);
        count += rw_result;
    }
   
    result = close(fd[1]);
    exit_on_error(result < 0, NULL, 0);

    print_result(bytes); 
    return EXIT_SUCCESS;
}


void use(const char* program)
{
    fprintf(stderr, "Use:\n  %s <bytes>\n", program);
    exit(EXIT_FAILURE);
}

void exit_on_argv_error(int argc, char** argv)
{
    if (argc != 2)
    {
        use(argv[0]);
    }
}

void exit_on_error(int assertion, error_callback callback, ssize_t count)
{
    if (assertion)
    {
        if (callback)
        {
            callback(count);
        }

        if (errno)
        {
            fprintf(stderr, "[%d]: %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        } 

        fprintf(stderr, "An error occured!\n");
        exit(EXIT_FAILURE);
    } 
}

void print_result(ssize_t value)
{
    fprintf(stdout, "Size of pipe: [%ld] bytes\n", value);
}

