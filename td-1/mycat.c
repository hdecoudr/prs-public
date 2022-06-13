/*!
 * \ingroup td_1_group
 * \file mycat.c
 * \brief Exercise 1.1
 *
 * Writes the standard input on the standard output.
 *
 * This program uses the following system calls:
 *
 * - [read(int fd, void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/read.2.html) 
 * - [write(int fd, const void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/write.2.html) 
 *
 * \author H. Decoudras
 * \version 1
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
 * Writes the standard input on the standard output.
 *
 * This program uses the following system calls:
 *
 * - [read(int fd, void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/read.2.html) 
 * - [write(int fd, const void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/write.2.html) 
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *            in case of error
 */
int main(void)
{
    char    buff;
    ssize_t rw_result;
    
    while (1)
    {
        /* Read the standard input */

        rw_result = read(STDIN_FILENO, &buff, sizeof(char));
        exit_on_error(rw_result < 0);

        if (rw_result == 0)
        {
            break;
        }

        /* Write to the standard output */

        rw_result = write(STDOUT_FILENO, &buff, 1);
        exit_on_error(rw_result < 0); 
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

