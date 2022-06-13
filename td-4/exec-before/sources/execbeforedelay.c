/*!
 * \ingroup td_4_group
 * \file execbeforedelay.c
 * \brief Exercise 4.8
 *  
 * \author H. Decoudras
 * \version 1
 */

#include <signal.h>
#include <unistd.h>
#include <setjmp.h>
#include <string.h>

#include "execbeforedelay.h"
#include "exiterror.h"


/*!
 * \brief The signal_handler() function restores
 *        the stack environment.
 *
 * \param sig Signal emitted.
 */
static void signal_handler(int sig);


/*!
 * \brief Backup of the stack environment including, optionally, 
 *        the signal mask. 
 */
static sigjmp_buf buf;


int exec_before_delay(void (*func)(void*), void* param, int delay)
{
    /* Backup the current stack environment */
    
    if (!sigsetjmp(buf, 1)) 
    {
        struct sigaction    act;
        int                 result;
        
        /* Set the handler */

        act.sa_handler = signal_handler;
        act.sa_flags = 0;

        /* 
            Initialize the signal mask 

            Set it to intercept all signals
         */

        result = sigemptyset(&act.sa_mask);
        exit_on_error(result < 0);

        /* Register the SIGALRM signal */

        result = sigaction(SIGALRM, &act, NULL);
        exit_on_error(result < 0);

        /* Trigger SIGALRM */

        alarm(delay);

        /* Execute the function */

        func(param);
        
        return 1;
    } 
    
    return 0;
}


void signal_handler(int sig) 
{
    /* restore the stack environment  */
    
    siglongjmp(buf, 1);
}

