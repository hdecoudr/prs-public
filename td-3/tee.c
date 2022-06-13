/*!
 * \ingroup td_3_group
 * \file tee.c
 * \brief Exercise 3.5
 *
 * Reads the content of the standard input and writes
 * it on the standard output and in a file.
 *
 * This program uses the following system calls: 
 *
 *  - [open(const char\* pathname, int flags, mode_t mode)](https://man7.org/linux/man-pages/man2/open.2.html)
 *  - [read(int fd, void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/read.2.html)
 *  - [write(int fd, const void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/write.2.html)
 *  - [close(int fd)](https://man7.org/linux/man-pages/man2/close.2.html)
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
 * \brief Size of the buffer.
 */
#define BUFFER_SIZE 4096


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
 * Reads the content of the standard input and writes
 * it on the standard output and in a file.
 *
 * This program uses the following system calls: 
 *
 *  - [open(const char\* pathname, int flags, mode_t mode)](https://man7.org/linux/man-pages/man2/open.2.html) 
 *  - [read(int fd, void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/read.2.html)
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
        Open the output file in write only mode

        Create the file if it does not exist and trunc
        its content
    */

    int fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0666);
    exit_on_error(fd < 0);

    char    buffer[BUFFER_SIZE];
    ssize_t count;
    ssize_t rw_result;

    /* 
        Display the content of the standard input 
        and write it to a file  */

    while ((count = read(STDIN_FILENO, buffer, 
           BUFFER_SIZE * sizeof(char))) > 0)
    {
        rw_result = write(fd, buffer, count * sizeof(char));
        exit_on_error(rw_result < 0);

        buffer[count] = '\0';

        rw_result = write(STDOUT_FILENO, buffer, count * sizeof(char));
        exit_on_error(rw_result < 0);
    }

    /* Close the file */

    int result = close(fd);
    exit_on_error(result < 0);

    return EXIT_SUCCESS;
}


void use(const char* program)
{
    fprintf(stderr, "Use:\n  %s <filename>\n", program);
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
            fprintf(stderr, "[%d]: %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }

        fprintf(stderr, "An error occured!\n");
        exit(EXIT_FAILURE);
    }
}

