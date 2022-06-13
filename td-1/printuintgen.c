/*!
 * \ingroup td_1_group
 * \file printuintgen.c
 * \brief Exercise 1.2
 *
 * Reads the `uintgen.bin` file, representing natural numbers, 
 * and displays its content on the standard output.
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
 * \brief File name.
 */
#define FNAME "./uintgen.bin"


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
 * \brief The uint_to_string() function converts a natural
 *        number into a string representation.
 *
 *        The string representation of the natural number 
 *        is stored in the \p buffer parameter and is
 *        inverted.
 *
 * \param buffer String representation of the natural number.
 * \param value Natural number to convert.
 *
 * \return The length of the string, including the null 
 *         terminating caracter.
 */
static size_t uint_to_string(char* buffer, unsigned int value);


/*!
 * \brief Main entry point of the program.
 *
 * Reads the `uintgen.bin` file, representing natural numbers, 
 * and displays its content on the standard output.
 *
 * This program uses the following system calls:
 *
 * - [open(const char\* pathname, int flags, mode_t mode)](https://man7.org/linux/man-pages/man2/open.2.html)
 * - [lseek(int fd, off_t offset, int whence)](https://man7.org/linux/man-pages/man2/lseek.2.html)
 * - [read(int fd, void\* buf, size_t count)](https://man7.org/linux/man-pages/man2/read.2.html) 
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
    int             fd;
    off_t           file_size;
    unsigned int    value_numeric;
    char            value_string[64];
    size_t          pos;
    off_t           seek_result;
    ssize_t         rw_result;
    
    fd = open(FNAME, O_RDONLY);
    exit_on_error(fd < 0);
   
    /* Get the file size */

    file_size = lseek(fd, 0, SEEK_END);
    exit_on_error(file_size < 0);
    
    /* Go back to the begining of the file */

    seek_result = lseek(fd, 0, SEEK_SET);
    exit_on_error(seek_result < 0);

    /* Display the content of the file */

    for (off_t i = 0; i < file_size; i += sizeof(unsigned int))
    {
        rw_result = read(fd, &value_numeric, sizeof(unsigned int));
        exit_on_error(rw_result < 0);
        
        pos = uint_to_string(value_string, value_numeric);
        while (pos)
        {
            rw_result = write(STDOUT_FILENO, &value_string[pos], sizeof(char));
            exit_on_error(rw_result < 0);
            --pos;
        }
    }

    fd = close(fd);
    exit_on_error(fd < 0);

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

size_t uint_to_string(char* buffer, unsigned int value)
{
    unsigned int    digit;
    size_t count    = 2;
    buffer[0]       = '\0';
    buffer[1]       = '\n'; 
    while (value)
    {
        digit = value % 0xA;
        buffer[count] = '0' + digit;
        value /= 0xA;
        ++count;
    }

    if (count == 2)
    {
        buffer[2] = '0';
    }
    else
    {
        --count;
    }

    return count;
}

