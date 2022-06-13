/*!
 * \ingroup td_1_group
 * \file mirroruint.c
 * \brief Exercise 1.4
 *
 * Generates the mirror of a file of natural numbers.
 *
 * This program uses the following system calls:
 * 
 * - [open(const char\* pathname, int flags, mode_t mode)](https://man7.org/linux/man-pages/man2/open.2.html) 
 * - [lseek(int fd, off_t offset, int whence)](https://man7.org/linux/man-pages/man2/lseek.2.html)
 * - [read(int fd, void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/read.2.html) 
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
 * \brief The use() function displays how to use the
 *        program.
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
 * Generates the mirror of a file of natural numbers.
 *
 * This program uses the following system calls:
 * 
 * - [open(const char\* pathname, int flags, mode_t mode)](https://man7.org/linux/man-pages/man2/open.2.html) 
 * - [lseek(int fd, off_t offset, int whence)](https://man7.org/linux/man-pages/man2/lseek.2.html)
 * - [read(int fd, void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/read.2.html) 
 * - [write(int fd, const void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/write.2.html) 
 * - [close(int fd)](https://man7.org/linux/man-pages/man2/close.2.html) 
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

    int             fd_in;
    int             fd_out;
    unsigned int    value_numeric;
    off_t           in_sz;
    int             result;
    off_t           seek_result;
    ssize_t         rw_result;

    /* Open the input file in read only mode */

    fd_in = open(argv[1], O_RDONLY);
    exit_on_error(fd_in < 0);
   
    /* Go to the end of the file */

    in_sz = lseek(fd_in, 0, SEEK_END);
    exit_on_error(in_sz < 0);
    
    /* 
        Open the output file in write only mode

        Create the file if it does not exist and trunc
        its content
    */

    fd_out = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0666);
    exit_on_error(fd_out < 0);
   
    /* Mirror */

    for (off_t offset = in_sz - sizeof(unsigned int); 
         offset >= 0; 
         offset -= sizeof(unsigned int))
    {
        /* Go back to the previous number */

        seek_result = lseek(fd_in, offset, SEEK_SET);
        exit_on_error(seek_result < 0);

        /* Read the number from the input file */

        rw_result = read(fd_in, &value_numeric, sizeof(unsigned int));
        exit_on_error(rw_result < 0);

        /* Write the number to the output file */

        rw_result = write(fd_out, &value_numeric, sizeof(unsigned int));
        exit_on_error(rw_result < 0);
    }

    /* Close the files */

    result = close(fd_in);
    exit_on_error(result < 0);

    result = close(fd_out);
    exit_on_error(result < 0);

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
            fprintf(stderr,"[%d]: %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        
        fprintf(stderr, "An error occured!\n");
        exit(EXIT_FAILURE);
    }
}

