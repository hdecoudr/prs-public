/*!
 * \ingroup td_4_group
 * \file main.c
 * \brief Exercise 4.9
 * 
 * Triggers a `SIGSEGV` and continue the execution.
 *
 * This program uses the following system calls and functions:
 *
 *  - [sigaction(int signum, const struct sigaction\* act, struct sigaction\* oldact)](https://man7.org/linux/man-pages/man2/sigaction.2.html)
 *  - [setjmp(jmp_buf env)](https://man7.org/linux/man-pages/man3/setjmp.3.html)
 *  - [longjmp(jmp_buf env, int val)](https://man7.org/linux/man-pages/man3/longjmp.3p.html)
 *
 * \author H. Decoudras
 * \version 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "execsafe.h"


/*!
 * \brief The f() function calls the g() function
 *        in order to trigger a `SIGSEGV`.
 *
 * \see g
 */
static void f(void);

/*!
 * \brief The g() function triggers a `SIGSEGV`. 
 */
static void g(void);

/*!
 * \brief Sample variable.
 */
static volatile int* a = NULL;


/*!
 * \brief Main entry point of the program.
 *
 * Triggers a `SIGSEGV` and continue the execution.
 *
 * This program uses the following system calls and functions:
 *
 *  - [sigaction(int signum, const struct sigaction\* act, struct sigaction\* oldact)](https://man7.org/linux/man-pages/man2/sigaction.2.html)
 *  - [setjmp(jmp_buf env)](https://man7.org/linux/man-pages/man3/setjmp.3.html)
 *  - [longjmp(jmp_buf env, int val)](https://man7.org/linux/man-pages/man3/longjmp.3p.html)
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *            in case of error 
 */
int main(void) 
{
    int result;
    result = test(f, SIGSEGV);
    if(result < 0) 
    {
        fprintf(stdout, "f() failed!\n");
    } 
    else 
    {
        printf("f() succeded!\n");
    }

    return EXIT_SUCCESS;
}


void f(void) 
{
    int result;
    result = test(g, SIGSEGV);
    if(result < 0) 
    {
        fprintf(stdout, "g() failded!\n");
    } 
    else 
    {
        fprintf(stdout, "g() succeded!\n");
    }
}

void g(void) 
{    
    /* Trigger SIGEGV */

    char x = 'a';
    *a = x;
}

