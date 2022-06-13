/*!
 * \ingroup td_6_group
 * \file shared2.c
 * \brief Exercise 6.3
 *
 * Uses a shared and a private variable.
 *
 * This program uses the following [OpenMP](https://www.openmp.org/)
 * directives:
 *
 *  - [parallel](https://www.openmp.org/spec-html/5.0/openmpse14.html#x54-800002.6)
 *  - [critical](https://www.openmp.org/spec-html/5.0/openmpsu89.html#x120-4470002.17.1)
 *
 * \author H. Decoudras
 * \version 1
 */

# include <omp.h>

#include <stdlib.h>
#include <stdio.h>


/*!
 * \brief Main entry point of the program.
 *
 * Uses a shared and a private variable.
 *
 * This program uses the following [OpenMP](https://www.openmp.org/)
 * directives:
 *
 *  - [parallel](https://www.openmp.org/spec-html/5.0/openmpse14.html#x54-800002.6)
 *  - [critical](https://www.openmp.org/spec-html/5.0/openmpsu89.html#x120-4470002.17.1)
 * 
 * \return [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 */
int main(void)
{

    /* Shared variable */

    int k = 0;

    /* Execute the following block in parallel */

    #pragma omp parallel
    {
        /* Private variable */

        int i;
        for(i = 0; i < 100000; ++i)
        {
            /* Mutual exclusion */
            
            #pragma omp crtitical
            {
                ++k;
            }
        }
    }
 
    fprintf(stdout, "Threads x 100000 = [%d]\n", k);
    return EXIT_SUCCESS;
}

