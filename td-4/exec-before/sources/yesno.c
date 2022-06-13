/*!
 * \ingroup td_4_group
 * \file yesno.c
 * \brief Exercise 4.8
 * 
 * This program asks the user twice to enter 
 *  `y`, `Y`, `n`, `N` two times. 
 *
 *  If, after 5 seconds, the user does not provide 
 *  one of the following answers mentioned above,
 *  or if a wrong answer is provided, then a `n` or 
 *  `N` answer is assumed.
 *
 *  This program uses the following functions:
 *
 *   - [sigsetjmp(sigjmp_buf,int)](https://man7.org/linux/man-pages/man3/sigsetjmp.3p.html)
 *   - [siglongjmp(sigjmp_buf,int)](https://man7.org/linux/man-pages/man3/siglongjmp.3p.html)
 *
 * \author H. Decoudras
 * \version 1
 */

#include <stdlib.h>
#include <stdio.h>

#include "execbeforedelay.h"


/*!
 * \brief Gets user input.
 *
 * \param answer User input.
 */
static void yes_or_no(void* answer);


/*!
 * \brief Main entry point of the program.
 *
 *  This program asks the user twice to enter 
 *  `y`, `Y`, `n`, `N` two times. 
 *
 *  If, after 5 seconds, the user does not provide 
 *  one of the following answers mentioned above,
 *  or if a wrong answer is provided, then a `n` or 
 *  `N` answer is assumed.
 *
 *  This program uses the following functions:
 *
 *   - [sigaction(int signum, const struct sigaction\* act, struct sigaction\* oldact)](https://man7.org/linux/man-pages/man2/sigaction.2.html)
 *   - [alarm(unsigned int seconds)](https://man7.org/linux/man-pages/man2/alarm.2.html)
 *   - [sigemptyset(sigset_t\* set)](https://man7.org/linux/man-pages/man3/sigemptyset.3p.html)
 *   - [sigsetjmp(sigjmp_buf,int)](https://man7.org/linux/man-pages/man3/sigsetjmp.3p.html)
 *   - [siglongjmp(sigjmp_buf,int)](https://man7.org/linux/man-pages/man3/siglongjmp.3p.html)
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *            in case of error
 */
int main(void)
{
    int answer;
    fprintf(stdout, "Yes or no [y/n]? ");
    if (exec_before_delay(yes_or_no, &answer, 5))
    {
        fprintf(stdout, "Answer: %c\n", answer == 1 ? 'Y' : 'N');
    }
    else
    {
        fprintf(stdout, "No answer!\n");
    }

    fprintf(stdout, "Yes or no [y/n]? ");
    if (exec_before_delay(yes_or_no, &answer, 5))
    {
        fprintf(stdout, "Answer: %c\n", answer == 1 ? 'Y' : 'N');
    }
    else
    {
        fprintf(stdout, "No answer!\n");
    }


    return EXIT_SUCCESS;
}


void yes_or_no(void* answer)
{
    char c = getchar(); 
    getchar();
    *(int *)answer = c == 'y' || c == 'Y';
}

