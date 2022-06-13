/*!
 * \ingroup td_4_group
 * \file parentchildsignals2.c
 * \brief Exercise 4.5
 *
 * Sends multiple signals to a parent process.
 *
 * This program uses the following system calls and functions:
 *
 *  - [sigaction(int signum, const struct sigaction\* act, struct sigaction\* oldact)](https://man7.org/linux/man-pages/man2/sigaction.2.html)
 *  - [sigprocmask(int how, const sigset_t\* set, sigset_t\* oldset)](https://man7.org/linux/man-pages/man2/sigprocmask.2.html)
 *  - [sigsuspend(const sigset_t\* mask)](https://man7.org/linux/man-pages/man2/sigsuspend.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html) 
 *  - [sigemptyset(sigset_t\* set)](https://man7.org/linux/man-pages/man3/sigemptyset.3p.html)
 *  - [sigfillset(sigset_t\* set)](https://man7.org/linux/man-pages/man3/sigfillset.3p.html)
 *  - [sigdelset(sigset_t\* set, int signo)](https://man7.org/linux/man-pages/man3/sigdelset.3p.html)
 *
 * \author H. Decoudras
 * \version 1
 */

#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
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
 * \brief The parent_handler() function displays
 *        a message when a signal is intercepted.
 *
 * \param sig Signal.
 */
static void parent_handler(int sig);

/*!
 * \brief The child_handler() function displays
 *        a message when a signal is intercepted.
 *
 * \param sig Signal.
 */
static void child_handler(int sig);

/*!
 * \brief The register_all_signals() function register
 *        all possible signals.
 *
 * \param act sigaction structure.
 */
static void register_all_signals(const struct sigaction* act);


/*!
 * \brief Number of signals triggered.
 */
static int signals[NSIG];


/*!
 * \brief Main entry point of the program.
 *
 * Sends multiple signals to a parent process.
 *
 * This program uses the following system calls and functions: 
 *
 *  - [sigaction(int signum, const struct sigaction\* act, struct sigaction\* oldact)](https://man7.org/linux/man-pages/man2/sigaction.2.html)
 *  - [sigprocmask(int how, const sigset_t\* set, sigset_t\* oldset)](https://man7.org/linux/man-pages/man2/sigprocmask.2.html)
 *  - [sigsuspend(const sigset_t\* mask)](https://man7.org/linux/man-pages/man2/sigsuspend.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html) 
 *  - [sigemptyset(sigset_t\* set)](https://man7.org/linux/man-pages/man3/sigemptyset.3p.html)
 *  - [sigfillset(sigset_t\* set)](https://man7.org/linux/man-pages/man3/sigfillset.3p.html)
 *  - [sigdelset(sigset_t\* set, int signo)](https://man7.org/linux/man-pages/man3/sigdelset.3p.html)
 *
 * \param argc Number of arguments of the program.
 * \param argv Arguments of the program.
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *            in case of error   */
int main(int argc, char** argv) 
{
    sigset_t    proc_mask;
    int         result;

    /*
        Initialise a signal mask

        Set it to ignore all signals
    */

    result = sigfillset(&proc_mask);
    exit_on_error(result < 0);

    /* Ignore all signals */

    result = sigprocmask(SIG_SETMASK, &proc_mask, NULL); 
    exit_on_error(result < 0);

    pid_t child_pid = fork();
    exit_on_error(child_pid < 0);

    if (!child_pid) 
    { 
        /* Child process */
 
        struct sigaction act;
        act.sa_handler = child_handler; 
        act.sa_flags = 0;
        
        /* 
            Initialize the signal mask
            
            Set it to intercept all signals
        */

        result = sigemptyset(&act.sa_mask);
        exit_on_error(result < 0);

        /* Register the SIGUSR1 signal */

        result = sigaction(SIGUSR1, &act, NULL);
        exit_on_error(result < 0);

        /* 
            Initialize a signal mask

            Set it to ignore all signals
         */

        sigset_t child_mask;
        result = sigfillset(&child_mask);
        exit_on_error(result < 0);

        /* Handle SIGUSR1 */

        result = sigdelset(&child_mask, SIGUSR1);
        exit_on_error(result < 0);

        pid_t parent_pid = getppid();
        int count = atoi(argv[1]);
 
        for (int i = 0; i < count; ++i)
        {
            for (int j = 2; j < argc ; ++j) 
            {
	            result = kill(parent_pid, atoi(argv[j]));
                exit_on_error(result < 0);
	            
                /* Wait for SIGUSR1 */

                sigsuspend(&child_mask); 
            }
        }	 
    
        sleep(5);
        result = kill(parent_pid, SIGKILL); 
        exit_on_error(result < 0);
    }
    else 
    { 
        /* Parent process */   
 
        struct sigaction act;
        act.sa_handler = parent_handler; 
        act.sa_flags = 0;
        
        /* 
            Initialize the signal mask 

            Set it to intercept all signals
        */

        result = sigemptyset(&act.sa_mask);
        exit_on_error(result < 0);

        /* Register all signals */

        register_all_signals(&act);
 
        /* 
            Initialize a signal mask 
            
            Set it to intercept all signal
        */

        sigset_t parent_mask;
        result = sigemptyset(&parent_mask);
        exit_on_error(result < 0);

        while (1) 
        {
            /* Wait for all signals */
            
            sigsuspend(&parent_mask);

            result = kill(child_pid, SIGUSR1);
            exit_on_error(result < 0);
        }
 
        /* Wait for the child process to finish */

        pid_t wait_result = wait(NULL);
        exit_on_error(wait_result < 0);
    }
 
    return EXIT_SUCCESS;
}


void use(const char* program)
{
    fprintf(stderr, "Use:\n  %s <count> [<signals>]\n", program);
    exit(EXIT_FAILURE);
}

void exit_on_argv_error(int argc, char** argv)
{
    if (argc < 3)
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

void parent_handler(int sig)
{
    fprintf(stdout, "[%2d, %d]: %s\n", getpid(), sig, strsignal(sig));
    ++signals[sig];
}

void child_handler(int sig)
{
    fprintf(stdout, "[%2d, %d]: %s\n", getpid(), sig, strsignal(sig));
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

