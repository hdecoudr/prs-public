/*!
 * \ingroup td_4_group
 * \file emitsignals.c
 * \brief Exercise 4.4
 *
 * Emits multiple signals to a process.
 *
 * This program uses the following system call:
 *
 *  - [kill(pid_t pid, int sig)](https://man7.org/linux/man-pages/man2/kill.2.html) 
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
 * Emits multiple signals to a process.
 *
 * This program uses the following system call:
 *
 *  - [kill(pid_t pid, int sig)](https://man7.org/linux/man-pages/man2/kill.2.html) 
 *
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

    int target_pid  = atoi(argv[1]);
    int count       = atoi(argv[2]);
    int result;

    for (int i = 0; i < count; ++i)
    {
        for (int j = 3; j < argc; ++j)
        {
            /* Sends the signal to the process */

            result = kill(target_pid, atoi(argv[j]));
            exit_on_error(result < 0);
            sleep(1);
        }
    }

    return EXIT_SUCCESS;   
}


void use(const char* program)
{
    fprintf(
        stderr, "Use:\n  %s <target_pid> <count> [<signals>]\n",
        program
    );
    exit(EXIT_FAILURE);
}

void exit_on_argv_error(int argc, char** argv)
{
    if (argc < 4)
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

