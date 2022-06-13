/*!
 * \ingroup td_4_group
 * \file longjmp3.c
 * \brief Exercise 4.7
 * 
 * Tries to save and restore the environment.
 *
 * This program uses the following functions:
 *
 *  - [setjmp(jmp_buf env)](https://man7.org/linux/man-pages/man3/setjmp.3.html)
 *  - [longjmp(jmp_buf env, int val)](https://man7.org/linux/man-pages/man3/longjmp.3p.html)
 *  
 * \author H. Decoudras
 * \version 1
 */

#include <setjmp.h>

#include <stdlib.h>
#include <stdio.h>


/*!
 * \brief The f() function restores the environment.
 */
static void f(void);

/*!
 * \brief The g() function saves the environment.
 */
static void g(void);


/*!
 * \brief Backup of the environment.
 */
static jmp_buf j_buffer;


/*!
 * \brief Main entry point of the program.
 *
 * Tries to save and restore the environment.
 *
 * This program uses the following functions:
 *
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
    volatile int i = 0;
    
    /* Backup the current environment */

    g();

    if(i > 9)
    {
        return EXIT_SUCCESS;
    }

    fprintf(stdout, "%d\n", i);
    
    ++i;
    
    /* 
        Fail

        The block where setjmp was set is not
        defined anymore
    */

    f();

    fprintf(stderr, "Failed!\n");
    return EXIT_FAILURE;
}


void f(void)
{
    longjmp(j_buffer, 1);
}

void g(void)
{
    setjmp(j_buffer);
}

