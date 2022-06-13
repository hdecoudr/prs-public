/*!
 * \ingroup td_4_group
 * \file sigint3.c
 * \brief Exercise 4.2
 *
 * Displays a message when a `SIGINT` is intercepted.
 *
 * This program uses the following system calls and functions:
 * 
 *  - [sigaction(int signum, const struct sigaction\* act, struct sigaction\* oldact)](https://man7.org/linux/man-pages/man2/sigaction.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [waitpid(pid_t pid, int\* wstatus, int options)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [pause(void)](https://man7.org/linux/man-pages/man2/pause.2.html)
 *  - [sigemptyset(sigset_t\* set)](https://man7.org/linux/man-pages/man3/sigemptyset.3p.html) 
 *
 * \author H. Decoudras
 * \version 1
 */

#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

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
 *        a message when a `SIGINT` is intercepted.
 *
 * \param sig Signal.
 */
static void signal_handler(int sig);


/*!
 * \brief Main entry point of the program.
 *
 * Displays a message when a `SIGINT` is intercepted.
 *
 * This program uses the following system calls and functions:
 *
 *  - [sigaction(int signum, const struct sigaction\* act, struct sigaction\* oldact)](https://man7.org/linux/man-pages/man2/sigaction.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [waitpid(pid_t pid, int\* wstatus, int options)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [pause(void)](https://man7.org/linux/man-pages/man2/pause.2.html)
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
    int res;
     
    pid_t child_pid = fork();
    exit_on_error(child_pid < 0);

    if (!child_pid)
    {
        /* Child process */
 
        struct sigaction act;

        /*
            Initialize the signal mask

            Set it to intercept all signals
         */

        res = sigemptyset(&act.sa_mask);
        exit_on_error(res < 0);

        /* Set the handler */

        act.sa_handler = signal_handler;
        act.sa_flags = 0;

        /* Register the SIGINT signal */

        res = sigaction(SIGINT, &act, NULL);
        exit_on_error(res < 0);

        /* Wait for a signal */

        pause();

        exit(EXIT_SUCCESS);
    }
    
    /* Parent process */

    /* Wait for the child process to finish */

    int status;
    res = waitpid(child_pid, &status, WUNTRACED | WCONTINUED);
    exit_on_error(res < 0);
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

void signal_handler(int sig)
{
    fprintf(stdout, " Ctrl-C detected!");
}

