/*!
 * \ingroup td_6_group
 * \file shared.c
 * \brief Exercise 6.3
 *
 * Uses a shared and a private variable.
 *
 * This program uses the following [OpenMP](https://www.openmp.org/)
 * directive:
 *
 *  - [parallel](https://www.openmp.org/spec-html/5.0/openmpse14.html#x54-800002.6)
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
 * Uses a shared and a private variable. 
 *
 * This program uses the following [OpenMP](https://www.openmp.org/)
 * directive:
 *
 *  - [parallel](https://www.openmp.org/spec-html/5.0/openmpse14.html#x54-800002.6) 
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
            ++k;       
        }
    }
 
    fprintf(stdout, "Threads x 100000 = [%d]\n", k);
    return EXIT_SUCCESS;
}

