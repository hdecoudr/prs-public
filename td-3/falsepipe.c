/*!
 * \ingroup td_3_group
 * \file falsepipe.c
 * \brief Exercise 3.1
 *
 * Simulates a pipe between two commands by using a file.
 * 
 * This program uses the following system calls and functions: 
 * 
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [execlp(const char\* file, const char\* arg, ..., NULL)](https://man7.org/linux/man-pages/man3/exec.3.html)
 *  - [execvp(const char\* file, char\* const argv[])](https://man7.org/linux/man-pages/man3/exec.3.html)
 *
 * \author H. Decoudras
 * \version 1
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*!
 * \brief Temporary file name.
 */
#define FNAME "./falsepipe.txt"


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
 * Simulates a pipe between two commands by using a file.
 * 
 * This program uses the following system calls and functions: 
 * 
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [wait(int\* wstatus)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [execlp(const char\* file, const char\* arg, ..., NULL)](https://man7.org/linux/man-pages/man3/exec.3.html)
 *  - [execvp(const char\* file, char\* const argv[])](https://man7.org/linux/man-pages/man3/exec.3.html)
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

    int child_pid = fork();
    exit_on_error(child_pid < 0);

    if (!child_pid)
    {
        /* Child process */

        /* Create the temporary file */

        int out_file = open(FNAME, O_CREAT | O_WRONLY | O_TRUNC, 0666);
        exit_on_error(out_file < 0);

        /* 
            Redirect the output file to the standard output 
            
            The standard output is silently closed and its
            file descriptor points to the output file
        */

        int result = dup2(out_file, STDOUT_FILENO);
        exit_on_error(result < 0);
        
        /* Close the output file */

        result = close(out_file);
        exit_on_error(result < 0);

        /* Execute the first command */

        result = execlp(argv[1], argv[1], NULL);
        exit_on_error(result < 0);
    }
    else
    {
        /* Parent process  */

        /* Wait for the child process to finish */

        int status;
        pid_t wait_result = wait(&status);
        exit_on_error(wait_result < 0);

        /* Open the temporary file */

        int in_file = open(FNAME, O_RDONLY);
        exit_on_error(in_file < 0);

        /* 
            Redirect the input file to the standard input

            The standard input is silently closed and its
            file descriptor points to the intput file
        */

        int result = dup2(in_file, STDIN_FILENO);
        exit_on_error(result < 0);

        /* Close the input file */

        result = close(in_file);
        exit_on_error(result < 0);

        /* Execute the second command */

        execvp(argv[2], argv + 2);
        exit_on_error(errno);
    }

    return EXIT_SUCCESS;
}


void use(const char* program)
{
    fprintf(stderr, "Use:\n  %s <cmd_1> <cmd_2> [<arguments>]\n", program);
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

