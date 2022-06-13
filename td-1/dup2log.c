/*!
 * \ingroup td_1_group
 * \file dup2log.c
 * \brief Exercise 1.9
 *
 * Redirects the standard error output to a file.
 *
 * This program uses the following system calls:
 *
 * - [open(const char\* pathname, int flags, mode_t mode)](https://man7.org/linux/man-pages/man2/open.2.html)
 * - [dup(int oldfd)](https://man7.org/linux/man-pages/man2/dup.2.html)
 * - [dup2(int oldfd, int newfd)](https://man7.org/linux/man-pages/man2/dup.2.html)
 * - [write(int fd, const void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/write.2.html)
 * - [close(int fd)](https://man7.org/linux/man-pages/man2/close.2.html) 
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
 * \brief Output file name.
 */ 
#define OUT_FILE "./error.log"


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
 * Redirects the standard error output to a file.
 *
 * This program uses the following system calls:
 *
 * - [open(const char\* pathname, int flags, mode_t mode)](https://man7.org/linux/man-pages/man2/open.2.html)
 * - [dup(int oldfd)](https://man7.org/linux/man-pages/man2/dup.2.html)
 * - [dup2(int oldfd, int newfd)](https://man7.org/linux/man-pages/man2/dup.2.html)
 * - [write(int fd, const void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/write.2.html)
 * - [close(int fd)](https://man7.org/linux/man-pages/man2/close.2.html)  
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *            in case of error
 */
int main(void)
{
    int     fd_stderr; 
    int     fd_out;
    char    buffer[23];
    int     result;
    ssize_t rw_result;

    /* 
        Save the standard error output file descriptor 
    */

    fd_stderr = dup(STDERR_FILENO);
    exit_on_error(fd_stderr < 0);

    /* 
        Open the output file in write only mode

        Create the file if it does not exist and trunc
        its content
    */

    fd_out = open(OUT_FILE, O_WRONLY | O_CREAT, 0666);
    exit_on_error(fd_out < 0);

    /* 
        Redirect the output file to the standard error
        output

        The standard error output is silently closed and
        its file descriptor points to the output file
    */

    result = dup2(fd_out, STDERR_FILENO);
    exit_on_error(result < 0);

    /* 
        Close the output file

        We will use STDERR_FILENO to refers to it
    */

    result = close(fd_out);
    exit_on_error(result < 0);

    /* Write to the output file */

    for (size_t i = 0; i < 5; ++i)
    {
        sprintf(buffer, "[%zu] An error occured!\n", i);
        rw_result = write(
            STDERR_FILENO, 
            buffer, 
            strlen(buffer) * sizeof(char)
        );
        exit_on_error(rw_result < 0);
    }
    
    /* Restore the stderr file descriptor */
    
    result = dup2(fd_stderr, STDERR_FILENO);
    exit_on_error(result < 0);
    
    const char* message = "STDERR restored!\n";
    rw_result = write(
        STDERR_FILENO, 
        message, 
        strlen(message) * sizeof(char)
    ); 
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

