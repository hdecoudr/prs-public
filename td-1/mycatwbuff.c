/*!
 * \ingroup td_1_group
 * \file mycatwbuff.c
 * \brief Exercise 1.7
 * 
 * Writes the standard input on the standard output by using a buffer.
 * 
 * \author H. Decoudras
 * \version 1
 */

#include <stdlib.h>
#include <stdio.h>


/*!
 * \brief Main entry point of the program.
 *
 * Writes the standard input on the standard output by using a buffer.
 *
 * \return [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 */
int main(void)
{
    /* It would be far more efficient to use a buffer of 4096 bytes */

    char buff[4];
    size_t count;
    while ((count = fread(&buff, sizeof(char), 4, stdin)) != 0)
    {
        fwrite(&buff, sizeof(char), count, stdout);
    }

    return EXIT_SUCCESS;
}

