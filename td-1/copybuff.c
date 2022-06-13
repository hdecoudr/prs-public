/*!
 * \ingroup td_1_group
 * \file copybuff.c
 * \brief Exercise 1.8
 *
 * Copies the content of a file to another one.
 *  
 * This program uses the following system calls:
 *
 * - [open(const char\* pathname, int flags, mode_t mode)](https://man7.org/linux/man-pages/man2/open.2.html)
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
 * \struct io_buffer
 * \brief The io_buffer structure represents a buffer.
 */
struct io_buffer
{
    /*!
     * \brief Bytes read.
     */
    unsigned char* values;

    /*!
     * \brief Number of bytes read.
     */
    ssize_t read_len;

    /*!
     * \brief Buffer size.
     */
    size_t buff_size;
};


/*!
 * \brief Type definition of the \ref io_buffer structure.
 *
 * \see io_buffer
 */
typedef struct io_buffer IOBuffer;


/*!
 * \brief The new_io_buffer() functions allocates a buffer.
 *
 * \param power Power of the binary logarithm computed in
 *              the function.
 *
 * \return An allocated buffer.
 */
static IOBuffer* new_io_buffer(size_t power);

/*!
 * \brief The delete_io_buffer() function de-allocates a buffer.
 *
 * \param buffer An allocated buffer.
 */
static void delete_io_buffer(IOBuffer* buffer);

/*!
 * \brief The get_io_buffer_value() function gets the bytes
 *        stored in the buffer.
 *
 * \param buffer An allocated buffer.
 * \param index Start index of the buffer.
 */
static unsigned char* get_io_buffer_value(const IOBuffer* buffer, 
                                          size_t index);

/*!
 * \brief The get_io_buffer_read_length() function gets the number
 *        of bytes read.
 *
 * \param buffer An allocated buffer.
 * 
 * \return The number of bytes read.
 */
static ssize_t get_io_buffer_read_length(const IOBuffer* buffer);

/*!
 * \brief The set_io_buffer_read_length() function sets the number 
 *        of bytes read.
 *
 * \param buffer An allocated buffer.
 * \param length Number of bytes read.
 */
static void set_io_buffer_read_length(IOBuffer* buffer, ssize_t length);

/*!
 * \brief The get_io_buffer_size() function gets the size of
 *        the buffer.
 *
 * \param buffer An allocated buffer.
 *
 * \return The size of the buffer.
 */
static size_t get_io_buffer_size(const IOBuffer* buffer);


/*!
 * \brief Main entry point of the program.
 *
 * Copies the content of a file to another one.
 *  
 * This program uses the following system calls:
 *
 * - [open(const char\* pathname, int flags, mode_t mode)](https://man7.org/linux/man-pages/man2/open.2.html)
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

    IOBuffer*   io_buffer;
    int         fd_in;
    int         fd_out;
    int         result;
    ssize_t     rw_result;
    
    io_buffer = new_io_buffer(strtoul(argv[3], NULL, 10));
    fprintf(stdout, "Buffer size: [%zu]\n", get_io_buffer_size(io_buffer));

    /* Open the input file in read only mode */

    fd_in = open(argv[1], O_RDONLY);
    exit_on_error(fd_in < 0);

    /* 
        Open the output file in write only mode

        Create the file if it does not exist and trunc
        its content
    */

    fd_out = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0666);
    exit_on_error(fd_out < 0);
    
    /* Read the input file */

    while ((rw_result = read(fd_in, 
           (void*)get_io_buffer_value(io_buffer, 0),
           get_io_buffer_size(io_buffer))) != 0)
    {
        exit_on_error(rw_result < 0);

        set_io_buffer_read_length(io_buffer, rw_result);
        
        /* Write to the output file */

        rw_result = write(
            fd_out, 
            get_io_buffer_value(io_buffer, 0),
            get_io_buffer_read_length(io_buffer)
        );
        exit_on_error(rw_result < 0);
    }

    delete_io_buffer(io_buffer);
    
    /* Close the files */

    result = close(fd_in);
    exit_on_error(result < 0);
    
    result = close(fd_out);
    exit_on_error(fd_out < 0);

    return EXIT_SUCCESS;
}


void use(const char* program)
{
    fprintf(
        stderr, 
        "Use:\n  %s <filename_in> <filename_out> "
        "<log2_of_buffer_length>\n",
        program
    );
    exit(EXIT_FAILURE);
}

void exit_on_argv_error(int argc, char** argv)
{
    if (argc != 4)
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

IOBuffer* new_io_buffer(size_t power)
{
    IOBuffer* buff = malloc(sizeof(IOBuffer));
    exit_on_error(buff == NULL);

    buff->buff_size = 1 << power;

    buff->values = malloc(buff->buff_size * sizeof(unsigned char));
    exit_on_error(buff->values == NULL);

    return buff;
}

void delete_io_buffer(IOBuffer* buffer)
{
    free(buffer->values);
    free(buffer);
}

unsigned char* get_io_buffer_value(const IOBuffer* buffer, size_t index)
{
    if (buffer->buff_size > index)
    {
        return (buffer->values + index);
    }

    return NULL;
}

ssize_t get_io_buffer_read_length(const IOBuffer* buffer) 
{
    return buffer->read_len;
}

void set_io_buffer_read_length(IOBuffer* buffer, ssize_t length)
{
    buffer->read_len = length;
}

size_t get_io_buffer_size(const IOBuffer* buffer)
{
    return buffer->buff_size;
}

