/*!
 * \ingroup td_4_group
 * \file trap.c
 * \brief Exercise 4.9
 * 
 * Intercepts the `SIGSEGV` signal.
 *
 * This program uses the following system calls and functions:
 *
 *  - [sigaction(int signum, const struct sigaction\* act, struct sigaction\* oldact)](https://man7.org/linux/man-pages/man2/sigaction.2.html)
 *  - [sigemptyset(sigset_t\* set)](https://man7.org/linux/man-pages/man3/sigemptyset.3p.html)
 *  
 * \author H. Decoudras
 * \version 1
 */

#include <signal.h>
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
 * \brief The signal_handler() function displays
 *        the signal intercepted on the standard
 *        output and updates the \ref a variable.
 *
 * \param sig Signal.
 */
static void signal_handler(int sig);

/*!
 * \brief Sample variable.
 */
static volatile char* a = NULL;

/*!
 * \brief Sample variable.
 */
static volatile char b = 'x';


/*!
 * \brief Main entry point of the program.
 *
 * Intercepts the `SIGSEGV` signal.
 *
 * This program uses the following system calls and functions:
 *
 *  - [sigaction(int signum, const struct sigaction\* act, struct sigaction\* oldact)](https://man7.org/linux/man-pages/man2/sigaction.2.html)
 *  - [sigemptyset(sigset_t\* set)](https://man7.org/linux/man-pages/man3/sigemptyset.3p.html)
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *            in case of error 
 */
int main(void)
{
    struct sigaction    act;
    char                x;
    int                 result;

    /* Set the handler */

    act.sa_handler = signal_handler;
    act.sa_flags = 0;
    
    /* 
        Initialize the signal mask

        set if to intercept all signals
    */

    result = sigemptyset(&act.sa_mask);
    exit_on_error(result < 0);
    
    /* Register the SIGSEGV signal */

    result = sigaction(SIGSEGV, &act, NULL);
    exit_on_error(result < 0);

    /* Trigger SIGSEV */

    x = *a;

    fprintf(stdout, "End: %c\n", x);
    return EXIT_SUCCESS;
}


void signal_handler(int sig)
{
    fprintf(stdout, "Signal [%2d]\n", sig);
    
    /* Ignored */

    a = &b;
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

