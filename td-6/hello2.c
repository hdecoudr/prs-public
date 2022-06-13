/*!
 * \ingroup td_6_group
 * \file hello2.c
 * \brief Exercise 6.1
 *
 * Displays messages in parallel on the standard output.
 *
 * This program uses the following [OpenMP](https://www.openmp.org/)
 * directives and functions:
 *
 *  - [parallel](https://www.openmp.org/spec-html/5.0/openmpse14.html#x54-800002.6)
 *  - [barrier](https://www.openmp.org/spec-html/5.0/openmpsu90.html#x121-4550002.17.2)
 *  - [omp_get_thread_num(void)]https://www.openmp.org/spec-html/5.0/openmpsu113.html#x150-6570003.2.4)
 *
 * \author H. Decoudras
 * \version 1
 */

#include <omp.h>

#include <stdlib.h>
#include <stdio.h>


/*!
 * \brief Main entry point of the program.
 *
 * Displays messages in parallel on the standard output.
 *
 * This program uses the following [OpenMP](https://www.openmp.org/)
 * directives and functions:
 *
 *  - [parallel](https://www.openmp.org/spec-html/5.0/openmpse14.html#x54-800002.6)
 *  - [barrier](https://www.openmp.org/spec-html/5.0/openmpsu90.html#x121-4550002.17.2)
 *  - [omp_get_thread_num(void)]https://www.openmp.org/spec-html/5.0/openmpsu113.html#x150-6570003.2.4)
 * 
 * \return [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 */
int main(void)
{
    /* Execute the following block in parallel */

    #pragma omp parallel
    {
        fprintf(stdout, "[%2d] Hello!\n", omp_get_thread_num());
        
        /* Wait for all thread */

        #pragma omp barrier

        /* Continue the execution */

        fprintf(stdout, "[%2d]   Bye!\n", omp_get_thread_num());
    }
    
    return EXIT_SUCCESS;
}

