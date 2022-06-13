/*!
 * \ingroup td_2_group
 * \file nchild.c
 * \brief Exercise 2.2
 *
 * Creates multiple child processes.
 *
 * This program uses the following system calls:
 *
 * - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 * - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html) 
 *
 * \author H. Decoudras
 * \version 1
 */

#include <sys/types.h>
#include <sys/wait.h>
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
 * Creates multiple child processes.
 *
 * This program uses the following system calls:
 *
 * - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 * - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html) 
 *
 * \param argc Number of arguments of the program.
 * \param argv Arguments of the program.
 *
 * \return [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *         in case of success and
 *         [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *         in case of error.
 */
int main(int argc, char** argv)
{
    exit_on_argv_error(argc, argv);

    int     child_count = atoi(argv[1]);
    pid_t   parent_pid = getpid();
    
    fprintf(stdout, "Parent process: [%d]\n", getpid());
    
    for (int i = 0; i < child_count; ++i)
    {
        pid_t child_pid = fork();
        exit_on_error(child_pid < 0);

        if (!child_pid)
	    {
            /* Child process  */

            fprintf(stdout, "Process [%d]: [%d]\n", i, getpid()); 

            /* Fork bomb if we do not exit the loop */

	        break;
	    }

        /* Parent process */
	}    

    if (getpid() == parent_pid)
    {
        int     status;
        pid_t   wait_result;

        /* Wait for all child processes to finish */

        for (int i = 0; i < child_count; ++i)
        {
            wait_result = wait(&status);
            exit_on_error(wait_result < 0);
            fprintf(stdout, "Terminated: [%d]\n", wait_result);
        }
    }

    return EXIT_SUCCESS;
}


void use(const char* program)
{
    fprintf(stderr, "Use:\n  %s <child_count>\n", program);
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

