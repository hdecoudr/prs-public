/*!
 * \ingroup td_4_group
 * \file execsafe.c
 * \brief Exercise 4.9
 *
 * \author H. Decoudras
 * \version 1
 */

#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>

#include "execsafe.h"
#include "exiterror.h"


/*!
 * \brief The signal_handler() function restores
 *        the stack environment.
 *
 * \param sig Signal emitted.
 */
static void signal_handler(int signo);


/*!
 * \brief Backup of the stack environment including, optionally, 
 *        the signal mask. 
 */
static sigjmp_buf buf;


int test(void (*func)(void), int sig)
{
    /* Backup the current stack environment */

    if (!sigsetjmp(buf, 1)) 
    {
        struct sigaction    act;
        int                 res;

        /* Set the handler */

        act.sa_handler = signal_handler;
        act.sa_flags = 0;

        /*
            Initialize the signal mask

            Set it to intercept all signals
         */

        res = sigemptyset(&act.sa_mask);
        exit_on_error(res < 0);

        /* Register the signal */

        res = sigaction(sig, &act, NULL);
        exit_on_error(res < 0);

        /* Execute the function */

        func();
        
        return 0;
    } 
    
    return -1;
}


void signal_handler(int signo) 
{
    siglongjmp(buf, 1);
}

