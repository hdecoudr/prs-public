/*!
 * \ingroup td_6_group
 * \file hello3.c
 * \brief Exercise 6.2
 *
 * Displays messages in parallel on the standard output.
 *
 * This program uses the following [OpenMP](https://www.openmp.org/)
 * directives and functions:
 *
 *  - [parallel](https://www.openmp.org/spec-html/5.0/openmpse14.html#x54-800002.6)
 *  - [critical](https://www.openmp.org/spec-html/5.0/openmpsu89.html#x120-4470002.17.1)
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
 *  - [critical](https://www.openmp.org/spec-html/5.0/openmpsu89.html#x120-4470002.17.1)
 *  - [omp_get_thread_num(void)]https://www.openmp.org/spec-html/5.0/openmpsu113.html#x150-6570003.2.4) 
 *
 * \return [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 */
int main(void)
{
    /* Execute the following block in parallel */

    #pragma omp parallel
    {
        /* Mutual exclusion */

        #pragma omp critical
        {
            fprintf(stdout, "[%2d] Hello!\n", omp_get_thread_num());
            fprintf(stdout, "[%2d]   Bye!\n", omp_get_thread_num());   
        }
    }

    return EXIT_SUCCESS;
}

