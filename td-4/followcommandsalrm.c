/*!
 * \ingroup td_4_group
 * \file followcommandsalrm.c
 * \brief Exercise 4.6
 * 
 * Executes multiple commands.
 *
 * This program uses the following system calls and functions:
 *
 *  - [sigaction(int signum, const struct sigaction\* act, struct sigaction\* oldact)](https://man7.org/linux/man-pages/man2/sigaction.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [waitpid(pid_t pid, int\* wstatus, int options)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [execvp(const char\* file, char\* const argv[])](https://man7.org/linux/man-pages/man3/exec.3.html)
 *  - [sigemptyset(sigset_t\* set)](https://man7.org/linux/man-pages/man3/sigemptyset.3p.html)
 *
 * \author H. Decoudras
 * \version 1
 */

#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*!
 * \brief Number of commands to execute.
 */
#define COMMAND_COUNT 4


/*!
 * \enum command_state
 * \brief The \ref command_state enumeration represents a
 *        commant state.
 */
enum command_state
{
    /*!
     * \brief The command has not been stated.
     */
    COMMAND_STATE_NOT_STARTED = 0,

    /*!
     * \brief The command is running.
     */
    COMMAND_STATE_RUNNING,

    /*!
     * \brief The command has been stopped.
     */
    COMMAND_STATE_STOPPED,

    /*!
     * \brief The command has been executed.
     */
    COMMAND_STATE_FINISHED
};


/*!
 * \brief Type definition of the \ref command_state
 *        enumeration
 *
 * \see command_state
 */
typedef enum command_state CommandState;


/*!
 * \brief String representation of the \ref command_state
 *        enumeration.
 *
 * \see command_state
 */
static const char* commands_str_list[4] = {
    "COMMAND_STATE_NOT_STARTED",
    "COMMAND_STATE_RUNNING",
    "COMMAND_STATE_STOPPED",
    "COMMAND_STATE_FINISHED"
};


/*!
 * \struct state
 * \brief The \ref state structure represents the
 *        state of command.
 *
 * \see command_state
 */
struct state
{
    /*!
     * \brief Process identifier.
     */
    pid_t pid;

    /*!
     * \brief Command name.
     */
    char command[10];

    /*!
     * \brief Arguments.
     */
    char arg[10];

    /*!
     * \brief State of the command.
     */
    CommandState state;
};


/*!
 * \brief Type definition of the \ref state
 *        structure
 *
 * \see state
 */
typedef struct state State;


/*!
 * \brief State of all the commands.
 *
 * \see state
 * \see command_state
 */
static State* states_list[COMMAND_COUNT];

/*!
 * \brief Commands to execute.
 */
static char* commands_list[COMMAND_COUNT][10] = {
    {"sleep", "1", NULL}, 
    {"sleep", "2", NULL}, 
    {"sleep", "3", NULL}, 
    {"sleep", "4", NULL}
};


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
 *        the state of each command on the standard
 *        output.
 *
 * \param sig Signal.
 */
static void signal_handler(int sig);

/*!
 * \brief The print_state() function displays the state of 
 *        each command on the standard output.
 *
 * \see command_state
 */
static void print_state(void);

/*!
 * \brief The pending_commands() function determines if all
 *        commands are terminated or not.
 *
 * \return This function can return the following values:
 *          - **1** if at least one command has not terminated
 *          - **0** if all commands have terminated
 *
 * \see command_state
 */
static int pending_commands(void);

/*!
 * \brief the timer() function sets a timer to emit
 *        a `SIGALRM` every half second.
 */
static void timer(void);


/*!
 * \brief Main entry point of the program.
 *
 * Executes multiple commands.
 *
 * This program uses the following system calls and functions:
 *
 *  - [sigaction(int signum, const struct sigaction\* act, struct sigaction\* oldact)](https://man7.org/linux/man-pages/man2/sigaction.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [waitpid(pid_t pid, int\* wstatus, int options)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [execvp(const char\* file, char\* const argv[])](https://man7.org/linux/man-pages/man3/exec.3.html)
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
    struct sigaction act;
    act.sa_handler = signal_handler;
    act.sa_flags = 0;
    
    /* 
        Initialize the signal mask 

        Set it to intercept all signals
    */

    sigemptyset(&act.sa_mask);
    exit_on_error(errno);

    /* Register the SIGALRM signal */

    sigaction(SIGALRM, &act, NULL);
    exit_on_error(errno);

    /* Setup the timer */

    timer();

    pid_t   child_pid;
    int     result;

    for (int i = 0; i < COMMAND_COUNT; ++i)
    {
        child_pid = fork();
        exit_on_error(child_pid < 0);
      
        if (!child_pid) 
        {  
            /* Child process */

            /* Execute a command */

            result = execvp(commands_list[i][0], commands_list[i]);
            exit_on_error(result < 0);
        } 
        
        /* Parent process */

        states_list[i] = malloc(sizeof(struct state));
        exit_on_error(states_list[i] == NULL);

        states_list[i]->pid = child_pid;
        strcpy(states_list[i]->command, commands_list[i][0]);
        strcpy(states_list[i]->arg, commands_list[i][1]);
        states_list[i]->state = COMMAND_STATE_RUNNING;
    }
  
    pid_t   wait_result;
    int     status;
    
    while (pending_commands())
    {
        /* Wait for a command to be executed */

        wait_result = waitpid(
            0, 
            &status, 
            WUNTRACED | WCONTINUED | WNOHANG
        );
        exit_on_error(wait_result < 0);

        /* Set the state of a command */

        if (wait_result > 0) 
        {
            int i;
	        for (i = 0; i < COMMAND_COUNT; ++i)
            {
                if (states_list[i]->pid == wait_result)
                {
                    break;
                }
            }
     
	        if (WIFEXITED(status))
            {
                states_list[i]->state = COMMAND_STATE_FINISHED;
	        } 
            else if (WIFSIGNALED(status)) 
            {
                states_list[i]->state = COMMAND_STATE_FINISHED;
	        } 
            else if (WIFSTOPPED(status))
            {
                states_list[i]->state = COMMAND_STATE_STOPPED;
	        } 
            else if (WIFCONTINUED(status)) 
            {
                states_list[i]->state = COMMAND_STATE_RUNNING;
	        }
        }
      
        sleep(1); 
    }
  
    print_state();
    printf("All processes terminated!\n");

    for (int i = 0; i < COMMAND_COUNT; ++i)
    {
        free(states_list[i]);
    }

    return EXIT_SUCCESS;
}


void signal_handler(int sig)
{
    if (sig == SIGALRM)
    {
        print_state();
    }
}

void print_state(void)
{ 
    for (int i = 0 ; i < COMMAND_COUNT; ++i)
    {
        switch (states_list[i]->state)
	    {
            case COMMAND_STATE_NOT_STARTED:
            {
	            fprintf(
                    stdout,
                    "[%d]: %s %s(%s)\n", 
                    states_list[i]->pid,
                    commands_str_list[states_list[i]->state],
                    states_list[i]->command,
                    states_list[i]->arg
                );

	            break;
            }

            case COMMAND_STATE_RUNNING:
            {
	            fprintf(
                    stdout,
                    "[%d]: %s %s(%s)\n", 
                    states_list[i]->pid,
                    commands_str_list[states_list[i]->state],
                    states_list[i]->command,
                    states_list[i]->arg
                );

	            break;
            }

	        case COMMAND_STATE_STOPPED:
            {
	            fprintf(
                    stdout,
                    "[%d]: %s %s(%s)\n", 
                    states_list[i]->pid,
                    commands_str_list[states_list[i]->state],
                    states_list[i]->command,
                    states_list[i]->arg
                );
	        
                break;
            }

            case COMMAND_STATE_FINISHED:
            {
	            fprintf(
                    stdout,
                    "[%d]: %s %s(%s)\n", 
                    states_list[i]->pid,
                    commands_str_list[states_list[i]->state],
                    states_list[i]->command,
                    states_list[i]->arg
                );
	            
                break;
            }
	    }
    }
  
    fprintf(stdout, "\n");
}

int pending_commands(void)
{ 
    for (int i = 0; i < COMMAND_COUNT; i++)
    {
        if (states_list[i]->state != COMMAND_STATE_FINISHED)
        {
	        return 1;
        }
    }

    return 0;
}

void timer(void)
{
    struct itimerval time;
    time.it_interval.tv_sec = 0;
    time.it_interval.tv_usec = 500000;
    time.it_value.tv_sec = 0;
    time.it_value.tv_usec = 500000;
    setitimer(ITIMER_REAL, &time, NULL);
}

void exit_on_error(int assertion)
{
    if (assertion)
    {
        if (errno == EINTR)
        {
            errno = 0;
            return;
        }

        if (errno) 
        {
            fprintf(stderr, "[%d]: %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        
        fprintf(stderr, "An error occured!\n");
        exit(EXIT_FAILURE);
    }
}

