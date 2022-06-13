/*!
 * \ingroup td_4_group
 * \file signalstring.c
 * \brief Exercise 4.1
 *
 * Displays the list of signals on the standard output.
 *
 * \author H. Decoudras
 * \version 1
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

/*!
 * \brief Main entry point of the program.
 * 
 * Displays the list of signals on the standard output.
 * 
 * \return [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html) 
 */
int main(void)
{
    for (int i = 0; i < NSIG; ++i)
    {
        fprintf(stdout, "[%2d]: %s\n", i, strsignal(i));
    }

    return EXIT_SUCCESS;
}

