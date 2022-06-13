/*!
 * \ingroup td_1_group
 * \file printstr.c
 * \brief Exercise 1.6
 *
 * Writes a string on the standard output by mixing 
 * a system call and a function of the standard library.
 *
 * This program uses the following system calls:
 *
 *  - [write(int fd, const void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/write.2.html)
 *
 * \author H. Decoudras
 * \version 1
 */

#include <unistd.h>
#include <errno.h>

#include <stdlib.h>
#include <stdio.h>
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
 * Writes a string on the standard output by mixing 
 * a system call and a function of the standard library.
 *
 * This program uses the following system calls:
 *
 *  - [write(int fd, const void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/write.2.html)
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *            in case of error
 */
int main(void)
{
    ssize_t rw_result;

    /* Write to the standard output */

    fprintf(stdout, "hello ");
    
    rw_result = write(STDOUT_FILENO, "world", 5 * sizeof(char));
    exit_on_error(rw_result < 0);

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

