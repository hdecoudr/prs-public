/*!
 * \ingroup td_4_group
 * \file getsignals.c
 * \brief Exercise 4.3
 *
 * Displays a message when a signal is received.
 *
 * This program uses the following system call and function:
 *
 *  - [sigaction(int signum, const struct sigaction\* act, struct sigaction\* oldact)](https://man7.org/linux/man-pages/man2/sigaction.2.html)
 *  - [sigemptyset(sigset_t\* set)](https://man7.org/linux/man-pages/man3/sigemptyset.3p.html)
 *
 * \author H. Decoudras
 * \version 1
 */

#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
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
 *        a message when a signal is intercepted.
 *
 * \param sig Signal.
 */
static void signal_handler(int sig);

/*!
 * \brief The register_all_signals() function register
 *        all possible signals.
 *
 * \param act sigaction structure.
 */
static void register_all_signals(const struct sigaction* act);


/*!
 * \brief Main entry point of the program.
 *
 * Displays a message when a signal is intercepted.
 *
 * This program uses the following system call and function:
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
    int                 result;
    
    /* 
        Initialize the signal mask 
        
        Set it to intercept all signals
    */

    result = sigemptyset(&act.sa_mask);
    exit_on_error(result < 0);

    /* Set the handler */

    act.sa_handler = signal_handler;
    act.sa_flags = 0;
    
    fprintf(stdout, "PID: [%d]\n", getpid());
    
    /* Register all signals */

    register_all_signals(&act);
    
    while (1)
    {
        sleep(1);
    }

    return EXIT_SUCCESS;   
}


void signal_handler(int sig)
{
    fprintf(stdout, "[%2d]: %s\n", sig, strsignal(sig));
}

void register_all_signals(const struct sigaction* act)
{
    /* 
        Some signals does not exists or it is not possible to
        attach an action to them.
    */
    
    int result;
    for (int i = 1; i < NSIG; ++i)
    { 
        result = sigaction(i, act, NULL);
        if (result < 0 && errno == EINVAL)
        {
            errno = 0;
        }
        else
        {
            exit_on_error(result < 0);
        }
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

