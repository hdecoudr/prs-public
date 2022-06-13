/*!
 * \ingroup td_2_group
 * \file commandlauncher.c
 * \brief Exercise 2.4
 *
 * Executes a shell command.
 *
 * This program use the following system calls and functions:
 *
 * - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 * - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 * - [execvp(const char\* file, char\* const argv[])](https://man7.org/linux/man-pages/man3/exec.3.html)
 *
 * \author H. Decoudras
 * \version 1
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


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
 * \brief The exec_command() function executes a shell
 *        command.
 *
 * \param commmand Shell command.
 */
static void command_launcher(char** argv);


/*!
 * \brief Main entry point of the program.
 *
 * This program use the following system calls and functions:
 *
 * - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 * - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 * - [execvp(const char\* file, char\* const argv[])](https://man7.org/linux/man-pages/man3/exec.3.html)
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
    command_launcher(&argv[1]);
    return EXIT_SUCCESS;
}


void use(const char* program)
{
    fprintf(stderr, "Use:\n  %s <command>\n", program);
    exit(EXIT_FAILURE);
}

void exit_on_argv_error(int argc, char** argv)
{
    if (argc < 2)
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

void command_launcher(char** argv)
{
    pid_t child_pid = fork();
    exit_on_error(child_pid < 0);

    if(!child_pid)
    {
        /* Child process */

        /* Execute the command */

        int result = execvp(argv[0], argv);

        /* Executed only if execvp fails  */
        
        exit_on_error(result < 0);
    }

    /* Parent process */

    /* Wait for the child process to finish */

    int status;
    pid_t wait_result = wait(&status);
    exit_on_error(wait_result < 0);

    fprintf(stdout, "Shell return status: [%d]\n", WEXITSTATUS(status));
}

