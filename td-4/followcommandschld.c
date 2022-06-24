/*!
 * \ingroup td_4_group
 * \file followcommandschld.c
 * \brief Exercise 4.6
 * 
 * Executes multiple commands.
 *
 * This program uses the following system calls and functions:
 *
 *  - [sigaction(int signum, const struct sigaction\* act, struct sigaction\* oldact)](https://man7.org/linux/man-pages/man2/sigaction.2.html)
 *  - [sigprocmask(int how, const sigset_t\* set, sigset_t\* oldset)](https://man7.org/linux/man-pages/man2/sigprocmask.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [waitpid(pid_t pid, int\* wstatus, int options)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [execvp(const char\* file, char\* const argv[])](https://man7.org/linux/man-pages/man3/exec.3.html)
 *  - [sigemptyset(sigset_t\* set)](https://man7.org/linux/man-pages/man3/sigemptyset.3p.html)
 *  - [sigaddset(sigset_t\* set, int signo)](https://man7.org/linux/man-pages/man3/sigaddset.3p.html)
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
 * \brief The signal_handler() function updates
 *        the state of a command.
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
 * \brief Main entry point of the program.
 *
 * Executes multiple commands.
 *
 * This program uses the following system calls and functions:
 *
 *  - [sigaction(int signum, const struct sigaction\* act, struct sigaction\* oldact)](https://man7.org/linux/man-pages/man2/sigaction.2.html)
 *  - [sigprocmask(int how, const sigset_t\* set, sigset_t\* oldset)](https://man7.org/linux/man-pages/man2/sigprocmask.2.html)
 *  - [fork(void)](https://man7.org/linux/man-pages/man2/fork.2.html)
 *  - [waitpid(pid_t pid, int\* wstatus, int options)](https://man7.org/linux/man-pages/man2/wait.2.html)
 *  - [execvp(const char\* file, char\* const argv[])](https://man7.org/linux/man-pages/man3/exec.3.html)
 *  - [sigemptyset(sigset_t\* set)](https://man7.org/linux/man-pages/man3/sigemptyset.3p.html)
 *  - [sigaddset(sigset_t\* set, int signo)](https://man7.org/linux/man-pages/man3/sigaddset.3p.html)
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *            in case of error  
 */
int main(int argc, char** argv)
{
    struct sigaction act;
    act.sa_handler = signal_handler;
    act.sa_flags = SA_RESTART; 
    
    /* Register the SIGCHLD signal */

    int result = sigaction(SIGCHLD, &act, NULL);
    exit_on_error(result < 0);

    /* 
        Initialize a signal mask

        Set it to intercept all signals
    */

    sigset_t mask;
    result = sigemptyset(&mask);
    exit_on_error(result < 0);

    /* 
        Add SIGCHLD to the mask 
        
        Do not handle SIGCHLD
    */

    result = sigaddset(&mask, SIGCHLD);
    exit_on_error(result < 0);

    pid_t child_pid;
    
    for (int i = 0; i < COMMAND_COUNT; ++i)
    { 
        /* Do not handle SIGCHLD */

        result = sigprocmask(SIG_BLOCK, &mask, NULL);
        exit_on_error(result < 0);
        
        child_pid = fork();
        exit_on_error(child_pid < 0);
      
        if (!child_pid) 
        {  
            /* Child process */
           
            /* Handle SIGCHLD */

            result = sigprocmask(SIG_UNBLOCK, &mask, NULL);
            exit_on_error(result < 0);

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

        /* Handle SIGCHILD */

        result = sigprocmask(SIG_UNBLOCK, &mask, NULL);
        exit_on_error(result < 0);
    }
    
    while (pending_commands())
    {
        print_state();
        sleep(1);
    }
  
    print_state();
    printf("All processes terminated!\n");

    exit(EXIT_SUCCESS);
}


void signal_handler(int sig)
{
    if (sig == SIGCHLD)
    {
        pid_t wait_result;  

        while ((wait_result = waitpid(-1, NULL, WNOHANG)) > 0)
        {
            for (int i = 0; i < COMMAND_COUNT; ++i)
            {
                if (states_list[i]->pid == wait_result)
                {
                    states_list[i]->state = COMMAND_STATE_FINISHED;
                }
            }
        }
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
        if (errno) 
        {
            fprintf(stderr, "[%d]: %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        
        fprintf(stderr, "An error occured!\n");
        exit(EXIT_FAILURE);
    }
}

