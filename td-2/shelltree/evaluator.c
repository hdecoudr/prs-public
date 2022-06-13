/*!
 * \ingroup td_2_group
 * \file evaluator.c
 * \brief Exercise 2.5
 *
 * Execute a shell command.
 *
 * \author H. Decoudras
 * \version 7
 */

#include "shelltree.h"
#include "evaluator.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>


/*!
 * \brief The report_error() function displays a message
 *        \p message if the \p assertion parameter is
 *        evaluated to `TRUE`.
 *
 * \param assertion Assertion to be evaluated.
 * \param message Message to be displayed.
 *
 * \return This function can return the following values:
 *          - **0** if no error was detected
 *          - **1** if an error has been detected
 */
static int report_error(int assertion, const char* message);

/*!
 * \brief The evaluate_simple_expression() function
 *        executes simple shell commands.
 *
 * \param e Shell command to be executed.
 * \param fd_list List of file descriptors currently in use.
 * \param is_background Determines if a shell command must be
 *        executed as a background task.
 *
 * \return This function can return the following values:
 *          - **0** if an error has been detected
 *          - **1** if no error was detected
 *
 * \see expression_type
 * \see expression
 */
static int evaluate_simple_expression(Expression* e, int* fd_list, 
                                      int is_background);

/*!
 * \brief The evaluate_redirection_expression() function
 *        executes a redirected shell command.
 *
 * \param type Type of shell command.
 * \param file File used for the redirection.
 * \param fd_list List of file descriptors currently in use.
 *
 * \return This function can return the following values:
 *          - **0** if an error has been detected
 *          - **1** if no error was detected
 *
 * \see expression_type
 * \see expression
 */
static int evaluate_redirection_expression(ExpressionType type, 
                                           const char* file, 
                                           int* fd_list);

/*!
 * \brief The evaluate_expression_recursive() function
 *        executes shell commands.
 *
 * \param e Shell commands to be executed.
 * \param fd_list List of file descriptors currently in use.
 * \param is_background Determines if a shell command must be
 *        executed as a background task.
 *
 * \return This function can return the following values:
 *          - **0** if an error has been detected
 *          - **1** if no error was detected
 *
 *  \see evaluate_simple_expression()
 *  \see evaluate_redirection_expression()
 *  \see expression_type
 *  \see expression
 */
static int evaluate_expression_recursive(Expression* e, int* fd_list, 
                                         int is_background);


int evaluate_expression(Expression* e)
{
    int fd_list[] = {
        STDIN_FILENO,
        STDOUT_FILENO,
        STDERR_FILENO
    };

    if (e->type == EMPTY)
    {
        return 0;
    }

    return evaluate_expression_recursive(e, fd_list, 0);
}


int report_error(int assertion, const char* message)
{
    if (assertion)
    {
        perror(message);
        return 1;
    }
  
    return 0;
}

int evaluate_simple_expression(Expression* e, int* fd_list, 
                               int is_background)
{
    int status = 0;
    pid_t child_pid = fork();

    if (!child_pid)
    {
        /* Child process */
      
        for (int i = 0; i < STDERR_FILENO + 1; ++i)
        {
	        if (fd_list[i] != i)
            {
	            dup2(fd_list[i], i);
            }
        }

        if (fd_list[0] != STDIN_FILENO) 
        {
            close(fd_list[0]);  
        }
      
        if (fd_list[1] != STDOUT_FILENO) 
        {
            close(fd_list[1]);  
        }

        if (fd_list[2] != STDERR_FILENO && fd_list[1] != fd_list[2]) 
        {
            close(fd_list[2]); 
        }

        execvp(e->arguments[0], e->arguments);
        perror(e->arguments[0]);
        exit(EXIT_FAILURE);      
    }
 
    /* Parent process */
  
    for (int i = 0; i < STDERR_FILENO + 1; ++i)
    {
        if (fd_list[i] != i)
        {
            if(i != STDERR_FILENO || fd_list[1] != fd_list[2])
            {
	            close(fd_list[i]);
            }
      
            fd_list[i] = i;
        }
    }
  
    if (!is_background)
    {
        waitpid(child_pid, &status, 0);
        status = WIFEXITED(status) ? WEXITSTATUS(status) : 
                    128 +  WTERMSIG(status);
    }

    return status;
}

int evaluate_redirection_expression(ExpressionType type, 
                                    const char* file, int* fd_list)
{  
    switch(type)
    {
        case REDIRECTION_I: 
        {
            int fd = open(file, O_RDONLY, 0666);
            if (report_error(fd < 0, file))
            {
                return 0;
            }

            fd_list[0] = fd; 
            break;
        }

        case REDIRECTION_O:
        {
            int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (report_error(fd < 0, file))
            {
                return 0;
            }

            fd_list[1] = fd;
            break;
        }

        case REDIRECTION_A:  
        {
            int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (report_error(fd < 0, file))
            {
                return 0;
            }

            fd_list[1] = fd; 
            break;
        }

        case REDIRECTION_E: 
        {
            int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (report_error(fd < 0, file))
            {
                return 0;
            }

            fd_list[2] = fd;
            break;
        }

        case REDIRECTION_EO:  
        {
            int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (report_error(fd < 0, file))
            {
                return 0;
            }

            fd_list[1] = fd;
            fd_list[2] = fd;
            break;
        }
        
        default:
        {
            break;
        }
         
    }
  
    return 1;
}

int evaluate_expression_recursive(Expression* e, int* fd_list, 
                                  int is_background)
{
    int status;
    int fd[2];
  
    if (e == NULL) 
    {
        return 0;
    }

    switch(e->type)
    {
        case EMPTY:
        {
            exit(EXIT_SUCCESS);
        }
    
        case SIMPLE:
        {
            return evaluate_simple_expression(
                e, 
                fd_list, 
                is_background
            );
        }

        case REDIRECTION_I: 
        case REDIRECTION_O: 	
        case REDIRECTION_A: 	
        case REDIRECTION_E: 	
        case REDIRECTION_EO:
        {
            if(!evaluate_redirection_expression(e->type, 
               e->arguments[0], fd_list))
            {
                return 0;
            }
    
            return evaluate_expression_recursive(
                e->left, 
                fd_list, 
                is_background
            );
        }
    
        case BACKGROUND:
        {
            return evaluate_expression_recursive(e->left, fd_list, 1);
        }
    
        case SEQUENCE:
        {
            evaluate_expression_recursive(e->left, fd_list, 0);
            return evaluate_expression_recursive(
                e->right, 
                fd_list, 
                is_background
            );
        }

        case SEQUENCE_OR:
        {
            if ((status = evaluate_expression_recursive(e->left, 
                fd_list, 0)))
            {
                status = evaluate_expression_recursive(
                    e->right, 
                    fd_list, 
                    0
                );
                return status;
            }

            break;
        }

        case SEQUENCE_AND:
        {
            if (!(status = evaluate_expression_recursive(e->left, 
                fd_list, 0)))
            {
                status = evaluate_expression_recursive(
                    e->right, 
                    fd_list, 
                    is_background
                );
            }

            return status;
        }

        case PIPE:
        {
            pipe(fd);

            {
	            int newfd_list[3];
	            newfd_list[0] = fd_list[0];
	            newfd_list[1] = fd[1];		
	            newfd_list[2] = fd_list[2];
	
	            evaluate_expression_recursive(e->left, newfd_list, 1);
            }
    
            {
	            int newfd_list[3] ;
	            newfd_list[0] = fd[0];
	            newfd_list[1] = fd_list[1];		
	            newfd_list[2] = fd_list[2];
	
    	        return evaluate_expression_recursive(
                    e->right, 
                    newfd_list, 
                    is_background
                );
            }  
        }

        default:
        {
            printf("Not implemented yet!\n");
            return 1;
        }
    }

    return 1;
}

