/*!
 * \ingroup td_3_group
 * \file doublegen.c
 * \brief Exercise 3.6
 *
 * Generates a file of rational numbers.
 *
 * This program uses the following system calls:
 * 
 *  - [open(const char\* pathname, int flags, mode_t mode)](https://man7.org/linux/man-pages/man2/open.2.html)
 *  - [write(int fd, const void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/write.2.html)
 *  - [close(int fd)](https://man7.org/linux/man-pages/man2/close.2.html)
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
 * \brief File name.
 */
#define FNAME "./doublegen.bin"


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
 * Generates a file of rational numbers.
 *
 * This program uses the following system calls:
 * 
 *  - [open(const char\* pathname, int flags, mode_t mode)](https://man7.org/linux/man-pages/man2/open.2.html)
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

    int k = atoi(argv[1]);

    /* 
        Open the output file in write only mode

        Create the file if it does not exist and trunc
        its content
    */

    int fd = open(FNAME, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    exit_on_error(fd < 0);

    double  d = 1.0;
    ssize_t rw_result; 

    /* Write to the output file */

    for (int i = 0; i < k; ++i)
    {
        rw_result = write(fd, &d, sizeof(double));
        exit_on_error(rw_result < 0);

        d *= 1.333;
    }

    /* Close the output file */

    int result = close(fd);
    exit_on_error(result < 0);

    return EXIT_SUCCESS;
}


void use(const char* program)
{
    fprintf(stderr, "Use:\n  %s <count>\n", program);
    exit(EXIT_FAILURE);
}

void exit_on_argv_error(int argc, char** argv)
{
    if (argc != 2)
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
            fprintf(stderr,"[%d]: %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        
        fprintf(stderr, "An error occured!\n");
        exit(EXIT_FAILURE);
    }
}

